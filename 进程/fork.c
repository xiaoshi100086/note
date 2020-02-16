// 代码说明
// 僵尸进程产生的原因：
// 一个进程在调用exit命令结束自己的生命的时候，其实它并没有真正的被销毁，而是留下一个称为僵尸进程（Zombie）的数据结构，并发送SIGCHLD信号给父进程。
// 它已经放弃了几乎所有内存空间，没有任何可执行代码，也不能被调度，仅仅在进程列表中保留一个位置，记载该进程的退出状态等信息供父进程收集，除此之外，僵尸进程不再占有任何内存空间。
// 如果父进程提前结束，则子进程则交给init进程接管，即init是子进程的父进程。init会捕获SIGCHLD信号并销毁僵尸进程。
// 如果子进程提前结束，则需要父进程显式的调用wait/waitpid/waitid销毁僵尸进程。可以有两种方法。

// 如下代码，运行如下进行编译
// gcc -o fork fork.c

// 直接运行会产生僵尸进程，可以使用如下两种方法避免僵尸进程

#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <time.h>

// 方法二：注册子进程销毁信号，并在信号处理函数中调用waitpid
// void signal_handler(int signo) {
//     if (signo == SIGCHLD) {
//         pid_t child;
//         int status;
//         while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
//             printf("child(%d) is exit, exit status is %d\n", child, WEXITSTATUS(status));
//         }
//     }
// }

int main(){
    // signal(SIGCHLD, signal_handler);

    pid_t fpid; // fpid表示fork函数返回的值
    int count = 0;
    fpid = fork();
    if (fpid < 0) printf("error in fork!");
    else if (fpid == 0) {
        printf("I am child. Process id is %d\n", getpid());
    }
    else {
        int status;
        printf("I am parent. Process id is %d\n", getpid());

        // 方法一：等待子进程结束，收集子进程信息并回收僵尸进程
        // start
        // pid_t child = waitpid(fpid, &status, 0);
        // printf("child(%d) is exit, exit status is %d\n", child, WEXITSTATUS(status));
        // end
    
        while(1);
    }
    return 0;
}

// fork() creates a new process by duplicating the calling process.  The
// new process is referred to as the child process.  The calling process
// is referred to as the parent process.

// The child process and the parent process run in separate memory
// spaces.  At the time of fork() both memory spaces have the same
// content.  Memory writes, file mappings (mmap(2)), and unmappings
// (munmap(2)) performed by one of the processes do not affect the
// other.
