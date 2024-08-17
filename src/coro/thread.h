/*
 * @file thread.h
 * @brief �߳���صķ�װ
 * @date 2024.6.17
 * @cite https://github.com/zhongluqiang/sylar-from-scratch/blob/main/sylar/thread.h
*/

#ifndef _THREAD_H_
#define _THREAD_H_

#include"mutex.h"

namespace coro{

/*
 * @brief �߳���
*/
class Thread : Noncopyable{
public:
    /// �߳�����ָ������
    typedef std::shared_ptr<Thread> ptr;

    /*
     * @brief ���캯��
     * @param[in] cb �߳�ִ�к���
     * @param[in] name �߳�����
    */
    Thread(std::function<void()> cb, const std::string &name);

    /*
     * @brief ��������
    */
    ~Thread();

    /*
     * @brief �߳�id
    */
    pid_t getID() const { return m_id; }

    /*
     * @brief �߳�����
    */
    const std::string &getName() const { return m_name; }

    /*
     * @brief �ȴ��߳�ִ�����
    */
    void join();

    /*
     * @brief ��ȡ��ǰ���߳�ָ��
    */
    static Thread *GetThis();

    /*
     * @brief ��ȡ��ǰ���߳�����
    */
    static const std::string &GetName();

    /*
     * @brief ���õ�ǰ�߳�����
     * @param[in] name �߳�����
    */
    static void SetName(const std::string &name);

private:
    /*
     * @brief �߳�ִ�к���
    */
    static void *run(void *arg);

private:
    /// �߳�id
    pid_t m_id = -1;
    /// �߳̽ṹ
    pthread_t m_thread = 0;
    /// �߳�ִ�к���
    std::function<void()> m_cb;
    /// �߳�����
    std::string m_name;
    /// �ź���
    Semaphore m_semaphore;
};

}

#endif
