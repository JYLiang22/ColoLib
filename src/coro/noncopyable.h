/*
 * @file noncopyable.h
 * @brief ���ɿ��������װ
 * @date 2024.6.17
 * @cite https://github.com/zhongluqiang/sylar-from-scratch/blob/main/sylar/noncopyable.h
*/

#ifndef _NONCOPYABLE_H_
#define _NONCOPYABLE_H_

namespace coro{

/*
    * @brief �����޷���������ֵ
*/
class Noncopyable{
public:
    /*
     * @brief Ĭ�Ϲ��캯��
    */
    Noncopyable() = default;

    /*
     * @brief Ĭ����������
    */
    ~Noncopyable() = default;

    /*
     * @brief �������캯��������
    */
    Noncopyable(const Noncopyable&) = delete;

    /*
     * @brief ��ֵ����������
    */
    Noncopyable operator=(const Noncopyable&) = delete;
};

}

#endif
