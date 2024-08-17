/*
 * @file fiber.cpp
 * @brief Э��ʵ��
 * @date 2024.6.13
 * @cite https://github.com/zhongluqiang/sylar-from-scratch/blob/main/sylar/fiber.cc
*/

#include<atomic>
#include<assert.h>
#include"fiber.h"

namespace coro{

/// ȫ�־�̬��������������Э��id
static std::atomic<uint64_t> s_fiber_id{0};
/// ȫ�־�̬����������ͳ�Ƶ�ǰЭ����
static std::atomic<uint64_t> s_fiber_count{0};

/// �ֲ߳̾���������ǰ�߳��������е�Э��
static thread_local Fiber *t_fiber = nullptr;
/// �ֲ߳̾���������ǰ�̵߳���Э�̣��л������Э�̣����൱���л��������߳������У�����ָ����ʽ
static thread_local Fiber::ptr t_thread_fiber = nullptr;

/// ����Ĭ��ջ�ռ�Ϊ128K
#define DEFAULT_STACK_SIZE (1024*128)

/*
 * @brief mallocջ�ڴ������
*/
class MallocStackAllocator{
public:
    static void *Alloc(size_t size) { return malloc(size); }
    static void Dealloc(void *vp, size_t size) { return free(vp); }
};

using StackAllocator = MallocStackAllocator;

uint64_t Fiber::GetFiberId(){
    if(t_fiber){
        return t_fiber->m_id;
    }
    return 0;
}

Fiber::Fiber(){
    SetThis(this);
    m_state = RUNNING;

    if(getcontext(&m_ctx)){
        /// �Ȳ���log��־ģ�飬��ô������ô��
        printf("Fiber::Fiber() getcontext failed\n");
        return;
    }

    ++s_fiber_count;
    m_id = s_fiber_id++;
    /// �Ȳ���log��־ģ�飬��ô������ô��
    printf("Fiber::Fiber() main id = %lu\n", m_id);
}

void Fiber::SetThis(Fiber *f){
    t_fiber = f;
}

Fiber::ptr Fiber::GetThis(){
    if(t_fiber){
        return t_fiber->shared_from_this();
    }
    
    Fiber::ptr main_fiber(new Fiber);
    assert(t_fiber == main_fiber.get());
    t_thread_fiber = main_fiber;
    return t_fiber->shared_from_this();
}

Fiber::Fiber(std::function<void()> cb, size_t stacksize, bool run_in_scheduler)
     : m_id(s_fiber_id++), m_cb(cb), m_runInScheduler(run_in_scheduler){
    ++s_fiber_count;
    m_stacksize = (stacksize ? stacksize : DEFAULT_STACK_SIZE);
    m_stack = StackAllocator::Alloc(m_stacksize);

    if(getcontext(&m_ctx)){
        /// �Ȳ���log��־ģ�飬��ô������ô��
        printf("Fiber::Fiber(param in) getcontext failed\n");
        return;
    }

    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    makecontext(&m_ctx, &Fiber::MainFunc, 0);

    /// �Ȳ���log��־ģ�飬��ô������ô��
    printf("Fiber::Fiber(param in) id = %lu\n", m_id);
}

/*
 * �̵߳���Э������ʱ��Ҫ���⴦����Ϊ��Э��û�з���ջ��cb
*/
Fiber::~Fiber(){
    printf("Fiber::~Fiber() deconstruct id = %lu\n", m_id);
    --s_fiber_count;
    if(m_stack){
        // ��ջ��˵��������Э�̣���Ҫȷ����Э���ǽ���״̬�ſ�������
        assert(m_state == TERM);
        StackAllocator::Dealloc(m_stack, m_stacksize);
        printf("Fiber::~Fiber() dealloc stack, id = %lu\n", m_id);
    }
    else{
        // û��ջ��˵�����̵߳���Э��
        assert(!m_cb && m_state == RUNNING);   // û��cb�ʹ�������״̬
        Fiber *cur = t_fiber;
        if(cur == this){
            SetThis(nullptr);
        }
    }
}

/*
 * Ϊ�˼�״̬��ǿ��ֻ��TERM״̬��Э�̲ſ������ã�Ҳ����ֻ����Э�̲ſ�������
 * ����Э�̾����ظ������ѽ�����Э�̣�������ջ�ռ䣬������Э��
*/
void Fiber::reset(std::function<void()> cb){
    assert(m_cb && m_state == TERM);   // m_cbȷ��Э����ջ����Э������Э��
    m_cb = cb;
    if(getcontext(&m_ctx)){
        printf("Fiber::reset(param in) getcontext failed\n");
        return;
    }

    m_ctx.uc_link = nullptr;
    m_ctx.uc_stack.ss_sp = m_stack;
    m_ctx.uc_stack.ss_size = m_stacksize;

    makecontext(&m_ctx, &Fiber::MainFunc, 0);
    m_state = READY;
}

void Fiber::resume(){
    assert(m_state == READY);
    SetThis(this);
    m_state = RUNNING;

    // if(swapcontext(&(t_thread_fiber->m_ctx), &m_ctx)){
    //     printf("Fiber::resume() swapcontext failed\n");
    //     return;
    // }

    // ���Э�̲�����������ȣ���ôӦ�ú͵���������Э�̽���swap���������߳���Э��
    if(m_runInScheduler){
        if(swapcontext(&(Scheduler::GetMainFiber()->m_ctx), &m_ctx)){
            printf("Fiber::resume() runInScheduler swapcontext failed\n");
            return;
        }
    }
    else{
        if(swapcontext(&(t_thread_fiber->m_ctx), &m_ctx)){
            printf("Fiber::resume() not runInScheduler swapcontext failed\n");
            return;
        }
    }
}

void Fiber::yield(){
    assert(m_state == RUNNING || m_state == TERM);
    SetThis(t_thread_fiber.get());
    if(m_state != TERM){
        m_state == READY;
    }

    // if(swapcontext(&m_ctx, &(t_thread_fiber->m_ctx))){
    //     printf("Fiber::yield() swapcontext failed\n");
    //     return;
    // }
    // ���Э�̲�����������ȣ���ôӦ�ú͵���������Э�̽���swap���������߳���Э��
    if(m_runInScheduler){
        if(swapcontext(&m_ctx, &(Scheduler::GetMainFiber()->m_ctx))){
            printf("Fiber::yield() runInScheduler swapcontext failed\n");
            return;
        }
    }
    else{
        if(swapcontext(&m_ctx, &(t_thread_fiber->m_ctx))){
            printf("Fiber::yield() not runInScheduler swapcontext failed\n");
            return;
        }
    }
}

void Fiber::MainFunc(){
    Fiber::ptr cur = GetThis();
    assert(cur);

    cur->m_cb();
    cur->m_cb = nullptr;
    cur->m_state = TERM;

    auto raw_ptr = cur.get();
    cur.reset();
    raw_ptr->yield();
}

}   // namespace coro
