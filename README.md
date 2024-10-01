# ColoLib

## 参考

[https://github.com/zhongluqiang/sylar-from-scratch](https://github.com/zhongluqiang/sylar-from-scratch)

## 协程库基础知识

开始之前，先了解一下什么是协程，可参考：<br>

1. [浅谈协程](https://jasonkayzk.github.io/2022/06/03/%E6%B5%85%E8%B0%88%E5%8D%8F%E7%A8%8B/)
2. [渡劫 C++ 协程](https://www.bennyhuo.com/2022/03/09/cpp-coroutines-01-intro/)

协程介绍：

1. [协程基本性质](doc/协程基本性质.md)
2. [C++20无栈协程的实现](doc/C++20无栈协程的实现.md)
3. [goroutine和libco解析](doc/有栈协程.md)

## 独立栈和共享栈的实现

1. [独立栈的实现](src/independentStack)
2. [共享栈的实现](src/sharedStack)
3. [参考](https://cloud.tencent.com/developer/article/1945309)

## ucontext族函数

正式开始编写协程库前，需要了解 Linux 下的 ucontext 族函数，并基于函数实现一个简单的协程库。

1. [ucontext-人人都可以实现的简单协程库](https://developer.aliyun.com/article/52886)
2. [代码实现](src/uthread)

## hook编程

## 优化
