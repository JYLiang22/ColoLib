/*
 * @file mutex.h
 * @brief �ź���������������д������Χ������������ԭ����
 * @date 2024.6.17
 * @cite https://github.com/zhongluqiang/sylar-from-scratch/blob/main/sylar/mutex.h
*/

#ifndef _MUTEX_H_
#define _MUTEX_H_

#include<thread>
#include<functional>
#include<memory>
#include<pthread.h>
#include<semaphore.h>
#include<stdint.h>
#include<atomic>
#include<list>
#include"noncopyable.h"

namespace coro{

/*
 * @brief �ź���
*/
class Semaphore : Noncopyable{
public:
    /*
     * @brief ���캯��
     * @param[in] count �ź���ֵ��С
    */
    Semaphore(uint32_t count  = 0);

    /*
     * @brief ��������
    */
    ~Semaphore();

    /*
     * @brief ��ȡ�ź���
    */
    void wait();

    /*
     * @brief �ͷ��ź���
    */
    void notify();

private:
    sem_t m_semaphore;
};

/*
 * @brief �ֲ�����ģ��ʵ��
*/
template<class T>
struct ScopedLockImpl{
public:
    /*
     * @brief ���캯��
     * @param[in] mutex Mutex
    */
    ScopedLockImpl(T &mutex) : m_mutex(mutex){
        m_mutex.lock();
        m_locked = true;
    }

    /*
     * @brief �����������Զ��ͷ���
    */
    ~ScopedLockImpl(){
        unlock();
    }

    /*
     * @brief ����
    */
    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }

    /*
     * @brief ����
    */
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    /// mutex
    T &m_mutex;
    /// �Ƿ�������
    bool m_locked;
};

/*
 * @brief �ֲ�����ģ��ʵ��
*/
template<class T>
struct ReadScopedLockImpl{
public:
    /*
     * @brief ���캯��
     * @param[in] mutex ��д��
    */
    ReadScopedLockImpl(T &mutex) : m_mutex(mutex){
        m_mutex.rdlock();
        m_locked = true;
    }

    /*
     * @brief �����������Զ��ͷ���
    */
    ~ReadScopedLockImpl(){
        unlock();
    }

    /*
     * @brief �϶���
    */
    void lock(){
        if(!m_locked){
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    /*
     * @brief �ͷ���
    */
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    /// mutex
    T &m_mutex;
    /// �Ƿ�������
    bool m_locked;
};

/*
 * @brief �ֲ�д��ģ��ʵ��
*/
template<class T>
struct WriteScopedLockImpl{
public:
    /*
     * @brief ���캯��
     * @param[in] mutex ��д��
    */
    WriteScopedLockImpl(T &mutex) : m_mutex(mutex){
        m_mutex.wrlock();
        m_locked = true;
    }

    /*
     * @brief ��������
    */
    ~WriteScopedLockImpl(){
        unlock();
    }

    /*
     * @brief ��д��
    */
    void lock(){
        if(!m_locked){
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    /*
     * @brief ����
    */
    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    /// Mutex
    T &m_mutex;
    /// �Ƿ�������
    bool m_locked;
};

/*
 * @brief ������
*/
class Mutex : Noncopyable{
public:
    /// �ֲ���
    typedef ScopedLockImpl<Mutex> Lock;

    /*
     * @brief ���캯��
    */
    Mutex(){
        pthread_mutex_init(&m_mutex, nullptr);
    }

    /*
     * @brief ��������
    */
    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }

    /*
     * @brief ����
    */
    void lock(){
        pthread_mutex_lock(&m_mutex);
    }

    /*
     * @brief ����
    */
    void unlock(){
        pthread_mutex_unlock(&m_mutex);
    }

private:
    /// mutex
    pthread_mutex_t m_mutex;
};

/*
 * @brief ����(���ڵ���)
*/
class NullMutex : Noncopyable{
public:
    /// �ֲ���
    typedef ScopedLockImpl<NullMutex> Lock;

    /*
     * @brief ���캯��
    */
    NullMutex() {}

    /*
     * @brief ��������
    */
    ~NullMutex() {}

    /*
     * @brief ����
    */
    void lock() {}

    /*
     * @brief ����
    */
    void unlock() {}
};

/*
 * @brief ��д������
*/
class RWMutex : Noncopyable{
public:

    /// �ֲ�����
    typedef ReadScopedLockImpl<RWMutex> ReadLock;

    /// �ֲ�д��
    typedef WriteScopedLockImpl<RWMutex> WriteLock;

    /*
     * @brief ���캯��
    */
    RWMutex(){
        pthread_rwlock_init(&m_lock, nullptr);
    }
    
    /*
     * @brief ��������
    */
    ~RWMutex(){
        pthread_rwlock_destroy(&m_lock);
    }

    /*
     * @brief �϶���
    */
    void rdlock(){
        pthread_rwlock_rdlock(&m_lock);
    }

    /*
     * @brief ��д��
    */
    void wrlock(){
        pthread_rwlock_wrlock(&m_lock);
    }

    /*
     * @brief ����
    */
    void unlock(){
        pthread_rwlock_unlock(&m_lock);
    }
private:
    /// ��д��
    pthread_rwlock_t m_lock;
};

/*
 * @brief �ն�д��(���ڵ���)
*/
class NullRWMutex : Noncopyable{
public:
    /// �ֲ�����
    typedef ReadScopedLockImpl<NullMutex> ReadLock;
    /// �ֲ�д��
    typedef WriteScopedLockImpl<NullMutex> WriteLock;

    /*
     * @brief ���캯��
    */
    NullRWMutex(){}

    /*
     * @brief ��������
    */
    ~NullRWMutex(){}

    /*
     * @brief �϶���
    */
    void rdlock() {}

    /*
     * @brief ��д��
    */
    void wrlock() {}

    /*
     * @brief ����
    */
    void unlock() {}
};

/*
 * @brief ������
*/
class Spinlock : Noncopyable{
public:
    /// �ֲ���
    typedef ScopedLockImpl<Spinlock> Lock;

    /*
     * @brief ���캯��
    */
    Spinlock(){
        pthread_spin_init(&m_mutex, 0);
    }

    /*
     * @brief ��������
    */
    ~Spinlock(){
        pthread_spin_destroy(&m_mutex);
    }

    /*
     * @brief ����
    */
    void lock(){
        pthread_spin_lock(&m_mutex);
    }

    /*
     * @brief ����
    */
    void unlock(){
        pthread_spin_unlock(&m_mutex);
    }
private:
    /// ������
    pthread_spinlock_t m_mutex;
};

/*
 * @brief ԭ����
*/
class CASLock : Noncopyable{
public:
    /// �ֲ���
    typedef ScopedLockImpl<CASLock> Lock;

    /*
     * @brief ���캯��
    */
    CASLock() {
        m_mutex.clear();
    }

    /*
     * @brief ��������
    */
    ~CASLock() {}

    /*
     * @brief ����
    */
    void lock(){
        while(std::atomic_flag_test_and_set_explicit(&m_mutex, std::memory_order_acquire));
    }

    /*
     * @brief ����
    */
    void unlock(){
        std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
    }
private:
    /// ԭ��״̬
    volatile std::atomic_flag m_mutex;
};

}

#endif
