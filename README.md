# ColoLib

## 参考

[https://github.com/zhongluqiang/sylar-from-scratch](https://github.com/zhongluqiang/sylar-from-scratch)

## 协程库基础知识

1. [协程基本性质](doc/协程基本性质.md)
2. [C++20无栈协程的实现](doc/C++20无栈协程的实现.md)
3. [经典有栈协程：goroutine和libco解析](doc/有栈协程.md)

## 独立栈和共享栈的实现

1. [参考](https://cloud.tencent.com/developer/article/1945309)
2. [独立栈的实现](src/independentStack)
3. [共享栈的实现](src/sharedStack)

## ucontext族函数

正式开始编写协程库前，需要了解 Linux 下的 ucontext 族函数，并基于函数实现一个简单的协程库。

1. 参考 [ucontext-人人都可以实现的简单协程库](https://developer.aliyun.com/article/52886)
2. [代码实现](src/uthread)

## hook编程

## 优化
