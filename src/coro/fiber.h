/*
 * @file fiber.h
 * @brief Э��ģ��
 * @details ����ucontext�庯��ʵ�ַǶԳ�Э��
 * @date 2024.6.12
 * @cite https://github.com/zhongluqiang/sylar-from-scratch/blob/main/sylar/fiber.h
*/

#ifndef _CORO_H_
#define _CORO_H_

#include<functional>
#include<memory>
#include<ucontext.h>

namespace coro{

/*
 * @brief Э����
*/
class Fiber : public std::enable_shared_from_this<Fiber>{
public:
    typedef std::shared_ptr<Fiber> ptr;

    /*
     * @brief Э��״̬
     * @details ֻ��������ת����ϵ���������� ׼������ ��������
    */
    enum State{
        /// @brief ����̬���մ�����yield֮���״̬
        READY,
        /// @brief ����̬��resume֮���״̬
        RUNNING,
        /// @brief ����̬��Э�̻ص�����ִ����֮��ΪTERM״̬
        TERM
    };

private:
    /*
     * @brief ���캯��
     * @attention �޲ι��캯��ֻ���ڴ����̵߳ĵ�һ��Э�̣�Ҳ�����߳���������Ӧ��Э��
     * ���Э��ֻ����GetThis()�������ã��������ó�˽�з���
    */
    Fiber();

public:
    /*
     * @brief ���캯�������ڴ����û�Э��
     * @param[in] cb Э����ں���
     * @param[in] stacksize ջ��С
     * @param[in] run_in_scheduler ��Э���Ƿ������������ȣ�Ĭ��Ϊtrue
    */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool run_in_scheduler = true);

    /*
     * @brief ��������
    */
    ~Fiber();

    /*
     * @brief ����Э��״̬����ں���������ջ�ռ䣬�����´���ջ
     * @param[in] cb
    */
    void reset(std::function<void()> cb);

    /*
     * @brief ����ǰЭ���л���ִ��״̬
     * @details ��ǰЭ�̺������������е�Э�̽��н�����ǰ�߱�ΪRUNNING�����߱�ΪREADY
    */
    void resume();

    /*
     * @brief ��ǰЭ���ó�ִ��Ȩ
     * @details ��ǰЭ�����ϴ�resume�˵���̨��Э�̽��н�����ǰ��״̬��ΪREADY�����߱�ΪRUNNING
    */
    void yield();

    /*
     * @brief ��ȡЭ��ID
    */
    uint64_t getId() const { return m_id; }

    /*
     * @brief ��ȡЭ��״̬
    */
    State getState() const { return m_state; }

public:
    /*
     * @brief ���õ�ǰ�������е�Э�̣��������ֲ߳̾�����t_fiber��ֵ
    */
    static void SetThis(Fiber *f);

    /*
     * @brief ���ص�ǰ�߳�����ִ�е�Э��
     * @details �����ǰ�߳�δ����Э�̣��򴴽��̵߳ĵ�һ��Э�̣��Ҹ�Э��Ϊ��ǰ�̵߳���Э�̣�����Э�̶�ͨ�����Э��������
     * Ҳ����˵������Э�̽���ʱ��Ҫ�ص���Э�̣�����Э������ѡ���µ�Э�̽���resume
     * @attention �߳����Ҫ����Э�̣���ôӦ������ִ��һ��Fiber::GetThis()�������Գ�ʼ��������Э��
    */
    static Fiber::ptr GetThis();

    /*
     * @brief ��ȡ��Э����
    */
    static uint64_t TotalFibers();

    /*
     * @brief Э����ں���
    */
    static void MainFunc();

    /*
     * @brief ��ȡ��ǰЭ��id
    */
    static uint64_t GetFiberId();

private:
    /// Э��id
    uint64_t m_id = 0;
    /// Э��ջ��С
    uint32_t m_stacksize = 0;
    /// Э��״̬
    State m_state = READY;
    /// Э��������
    ucontext_t m_ctx;
    /// Э��ջ��ַ
    void *m_stack = nullptr;
    /// Э����ں���
    std::function<void()> m_cb;
    /// Э���Ƿ�������������
    bool m_runInScheduler;
};

}   // namespace coro

#endif