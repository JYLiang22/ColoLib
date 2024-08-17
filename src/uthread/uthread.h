#ifndef _UTHREAD_H_
#define _UTHREAD_H_

#include<ucontext.h>
#include<vector>

#define DEFAULT_STACK_SIZE (1024*128)
#define MAX_UTHREAD_SIZE 1024

// Э�̵�����״̬���������� ���� ����ִ�� ����
enum ThreadState{
    FREE,
    RUNNABLE,
    RUNNING,
    SUSPEND
};

typedef void (*Fun)(void *arg);

// Э�̽ṹ��
typedef struct uthread_t{
    ucontext_t ctx;   // ����Э�̵�������
    Fun func;   // Э��ִ�е��û�����
    void *arg;   // func�Ĳ���
    enum ThreadState state;   // Э�̵�����״̬
    char stack[DEFAULT_STACK_SIZE];   // Э�̵�ջ
}uthread_t;

// Э�̵������ṹ��
typedef struct schedule_t{
    ucontext_t main;   // ��������������
    int running_thread;   // ����ִ�е�Э�̵ı��
    uthread_t *threads;   // ������ӵ�е�����Э��
    int max_index;   // Э���������

    schedule_t() : running_thread(-1), max_index(0){
        threads = new uthread_t[MAX_UTHREAD_SIZE];
        for(int i = 0; i < MAX_UTHREAD_SIZE; ++i){
            threads[i].state = FREE;
        }
    }

    ~schedule_t(){
        delete []threads;
    }
}schedule_t;

// Э��Э���ڵ�����������
static void uthread_body(schedule_t *ps);
// ����һ��Э�̣���Э�̵Ļ���뵽schedule��Э�������У�funcΪ��ִ�еĺ�����argΪfunc��ִ�к��������ش������߳���schedule�еı��
int uthread_create(schedule_t &schedule, Fun func, void *arg);
// ���������schedule�е�ǰ����ִ�е�Э�̣��л���������
void uthread_yield(schedule_t &schedule);
// �ָ����е�����schedule�б��Ϊid��Э��
void uthread_resume(schedule_t &schedule, int id);
// �ж�schedule�����е�Э���Ƿ�ִ����ϣ��Ƿ���1�����򷵻�0
int schedule_finished(const schedule_t &schedule);

#endif
