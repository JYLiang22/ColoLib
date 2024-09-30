# ColoLib


## 参考

[https://github.com/zhongluqiang/sylar-from-scratch](https://github.com/zhongluqiang/sylar-from-scratch)


## 协程库基础知识

开始之前，先了解一下什么是协程，可参考：<br>

1. [浅谈协程](https://jasonkayzk.github.io/2022/06/03/%E6%B5%85%E8%B0%88%E5%8D%8F%E7%A8%8B/)
2. [有栈协程与无栈协程](https://mthli.xyz/stackful-stackless/)
3. [浅谈有栈协程与无栈协程](https://zhuanlan.zhihu.com/p/347445164)
4. [渡劫 C++ 协程](https://www.bennyhuo.com/2022/03/09/cpp-coroutines-01-intro/)


## ucontext族函数

正式开始编写协程库前，需要了解 Linux 下的 ucontext 族函数，并基于函数实现一个简单的协程库。

1. [ucontext-人人都可以实现的简单协程库](https://developer.aliyun.com/article/52886)
2. [代码实现](src/uthread)


## 待实现

1. 如何优化
2. 比较和市面上各大协程库的区别：go、java、腾讯，并进一步深入学习C++协程的机制
3. 学习hook编程，就可以回答面试官问的问题了