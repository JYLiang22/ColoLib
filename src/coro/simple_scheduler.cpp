/*
 * @file simple_scheduler.cpp
 * @brief �򵥵�Э�̵�����ʵ��
 * @date 2024.6.14
 * @cite https://www.midlane.top/wiki/pages/viewpage.action?pageId=10060963
*/

#include<iostream>
#include<list>
#include"fiber.h"

/*
 * @brief ��Э�̵����֧࣬����ӵ�����������е�������
*/
class Scheduler{
private:
    /// �������
    std::list<coro::Fiber::ptr> m_tasks;

public:
    /*
     * @brief ���Э�̵�������
    */
    void schedule(coro::Fiber::ptr task){
        m_tasks.push_back(task);
    }

    /*
     * @brief ִ��Э�̵�������
    */
    void run(){
        coro::Fiber::ptr task;
        auto it = m_tasks.begin();
        while(it != m_tasks.end()){
            task = *it;
            m_tasks.erase(it++);
            task->resume();
        }
    }
};

void test_fiber(int i){
    std::cout << "hello " << i << std::endl;
}

int main(){

    /// ��ʼ����ǰ�̵߳���Э��
    coro::Fiber::GetThis();

    /// ����������
    Scheduler sc;

    /// ��ӵ�������
    std::cout << "add scheduler task" << std::endl;
    for(int i = 0; i < 10; ++i){
        coro::Fiber::ptr fiber(new coro::Fiber(std::bind(test_fiber, i)));
        std::cout << "add scheduler task " << i << std::endl;
        sc.schedule(fiber);
    }

    /// ִ�е�������
    sc.run();

    return 0;
}
