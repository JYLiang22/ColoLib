# hook

hook也叫钩子编程，一般来说是指程序 A 在程序 B 里埋入的一段代码，当程序 B 运行到某种特定状态，或者某个特定条件成立，就会调用 A 埋入的那段代码。<br>

hook编程有多种用途，例如：

1. 应用程序扩展：通过HOOK技术，我们可以在不修改原始代码的情况下，往现有的应用程序中加入自定义的功能。这种方式可以方便地实现各种插件、扩展和定制化需求。
2. 系统监控和调试：通过HOOK编程，可以在系统中拦截关键函数的调用并收集需要的数据，从而进行系统性能监控、错误日志记录和调试。这对于开发人员来说是非常有价值的。
3. 安全和防御：HOOK编程可以用于实现各种安全和防御策略。例如，我们可以通过HOOK技术来监控系统调用，检测恶意软件的行为，并拦截攻击行为的发生。
4. 当然也被用于恶意代码，例如游戏外挂。

HOOK编程可以应用于不同层次的软件开发，包括**操作系统级别、应用程序级别以及第三方库和插件级别**等。常见的应用场景包括：

1. 系统级HOOK：通过修改操作系统API或内核驱动程序来实现对系统行为的监控和修改。例如，可以通过HOOK技术实现对系统资源、进程、网络通信等的监控，实现杀毒软件、防火墙、调试工具等的功能。
2. 应用程序级HOOK：针对特定应用程序的行为进行改变。例如，可以通过HOOK技术实现对应用程序的界面修改、流量拦截、快捷键替换等。
3. 第三方库和插件级别HOOK：通过HOOK技术实现对第三方库或插件的行为进行修改或扩展。例如，可以通过HOOK技术对第三方库的函数进行拦截，实现对其行为的修改或增加特定功能。

HOOK编程通常分为两种类型：**静态HOOK和动态HOOK**。

1. 静态HOOK：静态HOOK是指在软件编译和链接阶段就将挂钩函数与目标函数进行绑定。这种方式通常需要修改目标程序的源代码，或者使用特定的工具进行重定向操作。静态HOOK的优点是稳定可靠，但对于已编译的软件来说，改变起来比较困难。
2. 动态HOOK：动态HOOK是在软件运行时对函数进行挂钩。这种方式通过修改函数的入口地址或者修改相关数据结构来实现。动态HOOK的优点是可以对已编译的软件进行修改，但相对于静态HOOK来说，实现和维护起来更为复杂。
    
    例子如下：<br>
    ```cpp
    // noninvadehook.c
    #include<unistd.h>
    #include<string.h>
    #include<sys/syscall.h>

    ssize_t write(int fd, const void *buf, size_t count){
        syscall(SYS_write, STDOUT_FILENO, "12345\n", strlen("12345\n"));
    }

    int main(){

        write(STDOUT_FILENO, "hello world\n", strlen("hello world\n"));
        return 0;
    }
    ```
    输出如下：<br>
    ```shell
    12345
    ```
    
    例子如下：<br>
    ```cpp
    // invadehook.c
    #include<unistd.h>
    #include<string.h>

    int main(){

        write(STDOUT_FILENO, "hello world\n", strlen("hello world\n"));
        return 0;
    }

    // hook.c
    #include<unistd.h>
    #include<sys/syscall.h>
    #include<string.h>

    ssize_t write(int fd, const void *buf, size_t count){
        syscall(SYS_write, STDOUT_FILENO, "12345\n", strlen("12345\n"));
    }
    ```
    将hook.c编译成动态库：`gcc -fPIC -shared hook.c -o libhook.so`；通过设置 LD_PRELOAD 环境变量，将libhoook.so设置成优先加载，从面覆盖掉libc中的write函数，如下：<br>
    ```shell
    LD_PRELOAD="./libhook.so" ./invahook
    12345
    ```

HOOK编程可以有多种实现方式，以下是几种常见的方式：

1. API Hooking：这是最常用的HOOK技术之一，通过修改或替换应用程序中的函数指针来实现。通过这种方式，我们可以在函数的执行前后插入自定义的代码，从而改变函数的行为。
2. DLL Injection：这种方式通过将自定义的DLL注入到目标进程中，来实现对目标进程的HOOK。通过这种方式，我们可以替换目标进程中的函数或在函数的执行前后插入自定义代码。
3. VTable Hooking：VTable是虚函数表的缩写，针对C++编程语言中的虚函数，可以通过修改对象的VTable指针来实现函数的HOOK。

关于hook的另一个重要的点在于如何找回已经被全局符号介入机制覆盖的系统调用接口，这里只需要知道是通过 `dlsym`，可以通过 `man 3 dlsym` 查询，函数原型是 `void *dlsym(void *handle, const char *symbol);`，第一个参数固定为 `RTLD_NEXT`，第二个是要查询的符号的名称。具体详细的可以看[博客](https://www.midlane.top/wiki/pages/viewpage.action?pageId=16417219#hook%E6%A8%A1%E5%9D%97-hook%E5%AE%9E%E7%8E%B0)。<br>


# 静态库和动态库

可以参考自己做的[笔记](https://github.com/JYLiang22/TinyWebServer/blob/main/project/Linux%E9%AB%98%E5%B9%B6%E5%8F%91%E6%9C%8D%E5%8A%A1%E5%99%A8%E5%BC%80%E5%8F%91/1%20Linux%E7%B3%BB%E7%BB%9F%E7%BC%96%E7%A8%8B%E5%85%A5%E9%97%A8/%E7%AC%94%E8%AE%B0.md#gcc)，已经介绍得很清楚了。<br>
同时还要参考[博客](https://www.midlane.top/wiki/pages/viewpage.action?pageId=16418206)。<br>
