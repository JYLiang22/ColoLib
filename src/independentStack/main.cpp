/*
 * @author: JYLiang22
 * @date: 2024-09-30
 * @description: 测试文件
*/

#include"coroutine.h"
#include<stdio.h>

void func1(void *arg)
{
    schedule_t *ps=static_cast<schedule_t *>(arg);
    printf("func1 first line: ########\n");
    coroutine_yield(*ps);
    printf("func1 second line: ########\n");
}

void func2(void *arg)
{
    schedule_t *ps=static_cast<schedule_t *>(arg);
    printf("func2 first line: *********\n");
    coroutine_yield(*ps);
    printf("func2 second line: *********\n");
}

int main()
{
    schedule_t schedule;
    int id1=coroutine_create(schedule,func1,&schedule);
    int id2=coroutine_create(schedule,func2,&schedule);
    while(is_schedule_finished(schedule)!=1)
    {
        coroutine_resume(schedule,id1);
        coroutine_resume(schedule,id2);
    }
    return 0;
}