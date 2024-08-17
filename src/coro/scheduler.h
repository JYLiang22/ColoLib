/*
 * @file scheduler.h
 * @brief Э�̵�����ʵ��
 * @date 2024.6.18
 * @cite https://github.com/zhongluqiang/sylar-from-scratch/blob/main/sylar/scheduler.h
*/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include<functional>
#include<vector>
#include<list>
#include<memory>
#include<string>
#include"fiber.h"
#include"thread.h"

namespace coro{

/*
 * @brief Э�̵�����
 * @details ��װ����һ��N-M��Э�̵��������ڲ���һ���̳߳أ�֧��Э�����̳߳������л�
*/
class Scheduler{
public:
    typedef std::shared_ptr<Scheduler> ptr;
    typedef Mutex MutexType;

    /*
     * @brief ����������
     * @prram[in] threads �߳���
     * @param[in] use_caller �Ƿ񽫵�ǰ�߳�Ҳ��Ϊ�����߳�
     * @param[in] name ����
    */
    Scheduler(size_t threads = 1, bool use_caller = true, const std::string &name = "Scheduler");

    /*
     * @brief ��������
    */
    virtual ~Scheduler();

    /*
     * @brief ��ȡ����������
    */
    const std::string &getName() const { return m_name; }

    /*
     * @brief ��ȡ��ǰ�̵߳�����ָ��
    */
    static Scheduler *GetThis();

    /*
     * @brief ��ȡ��ǰ�̵߳���Э��
    */
    static Fiber *GetMainFiber();

    /*
     * @brief ��ӵ�������
     * @tparam FiberOrCb �����������ͣ�������Э�̶������ָ��
     * @param[] fc Э�̶����ָ��
     * @param[] thread ָ�����и�������̺߳ţ�-1��ʾ�����߳�
    */
    template<class FiberOrCb>
    void schedule(FiberOrCb fc, int thread = -1){
        bool need_tickle = false;
        {
            MutexType::Lock lock(m_mutex);
            need_tickle = scheduleNoLock(fc, thread);
        }

        if(need_tickle){
            tickle();   // ����idleЭ��
        }
    }

    /*
     * @brief ����������
    */
    void start();

    /*
     * @brief ֹͣ�������������е�������ִ�������ٷ���
    */
    void stop();

protected:
    /*
     * @brief ֪ͨЭ�̵�������������
    */
    virtual void tickle();

    /*
     * @brief Э�̵��Ⱥ���
    */
    void run();

    /*
     * @brief ���������ʱִ��idleЭ��
    */
    virtual void idle();

    /*
     * @brief �����Ƿ����ֹͣ
    */
    virtual bool stopping();

    /*
     * @brief ���õ�ǰ��Э�̵�����
    */
    void setThis();

    /*
     * @brief �����Ƿ��п���Э��
     * @details ������Э�̽���idleʱ�����߳�����1����idleЭ�̷�����ʱ�����߳�����1
    */
    bool hasIdleThreads() { return m_idleThreadCount > 0; }

private:
    /*
     * @brief ��ӵ�����������
     * @tparam FiberOrCb �����������ͣ�������Э�̶������ָ��
     * @param[] fc Э�̶����ָ��
     * @param[] thread ָ�����и�������̺߳ţ�-1��ʾ�����߳�
    */
    template<class FiberOrCb>
    bool scheduleNoLock(FiberOrCb fc, int thread){
        bool need_tickle = m_tasks.empty();
        ScheduleTask task(fc, thread);
        if(task.fiber || tack.cb){
            m_tasks.push_back(task);
        }
        return need_tickle;
    }

private:
    /*
     * @brief ��������Э��/������ѡһ����ָ�����ĸ��߳��ϵ���
    */
    struct ScheduleTask{
        Fiber::ptr fiber;
        std::function<void()> cb;
        int thread;

        ScheduleTask(Fiber::ptr f, int thr){
            fiber = f;
            thread = thr;
        }

        ScheduleTask(Fiber::ptr *f, int thr){
            fiber.swap(*f);
            thread = thr;
        }

        ScheduleTask(std::function<void()> f, int thr){
            cb = f;
            thread = thr;
        }

        ScheduleTask() { thread = -1; }

        void reset(){
            fiber = nullptr;
            cb = nullptr;
            thread = -1;
        }
    };

private:
    /// Э�̵���������
    std::string m_name;
    /// ������
    MutexType m_mutex;
    /// �̳߳�
    std::vector<Thread::ptr> m_threads;
    /// �������
    std::list<ScheduleTask> m_tasks;
    /// �̳߳ص��߳�ID����
    std::vector<int> m_threadIds;
    /// �����߳�������������use_caller�����߳�
    size_t m_threadCount = 0;
    /// ��Ծ�߳���
    std::atomic<size_t> m_activeThreadCount = {0};
    /// idle�߳���
    std::atomic<size_t> m_idleThreadCount = {0};

    /// �Ƿ�use caller
    bool m_useCaller;
    /// use_callerΪtrueʱ�������������̵߳ĵ���Э��
    Fiber::ptr m_rootFiber;
    /// use_callerΪtrueʱ�������������̵߳�id
    int m_rootThread = 0;

    /// �Ƿ�����ֹͣ
    bool m_stopping = false;
};

}

#endif