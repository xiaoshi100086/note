// 代码说明

// 如下代码，运行如下进行编译
// gcc -o clone clone.c

// clone描述
// These system calls create a new ("child") process, in a manner
// similar to fork(2).

// By contrast with fork(2), these system calls provide more precise
// control over what pieces of execution context are shared between the
// calling process and the child process.  For example, using these
// system calls, the caller can control whether or not the two processes
// share the virtual address space, the table of file descriptors, and
// the table of signal handlers.  These system calls also allow the new
// child process to be placed in separate namespaces(7).

#define _GNU_SOURCE   /*注意摆放位置*/ 
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <wait.h>
#define gettid() syscall(SYS_gettid)

int variable,fd;
int do_something() {
    printf("child process pid is %d, tid is %d\n", getpid(), gettid());
    variable++;
    close(fd);
    // _exit(0);
    return 0;
}

// 方法二：注册子进程销毁信号，并在信号处理函数中调用waitpid
void signal_handler(int signo) {
    if (signo == SIGCHLD) {
        pid_t child;
        int status;
        while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
            printf("child(%d) is exit, exit status is %d\n", child, WEXITSTATUS(status));
        }
    }
}
 
int main(int argc, char *argv[]) {
    signal(SIGCHLD, signal_handler);
    void *child_stack = (void *)malloc(16384);
    char temp;
    variable = 42;
    fd = open("./clone.c",O_RDONLY);
    printf("parent process pid is %d, tid is %d\n", getpid(), gettid());
    printf("The variable was %d\n",variable);

    // 注意第二个参数，堆栈是向上生长的，所以需要给出堆栈的尾指针
    clone(do_something, child_stack+10000, CLONE_FILES|CLONE_VM|CLONE_SIGHAND|CLONE_FS, NULL);
    sleep(3);        /* 延时以便子进程完成关闭文件操作、修改变量*/

    printf("The variable is %d\n",variable);
    if (read(fd,&temp,1) < 1) {
        printf("can not read from the file\n");
    } else {
        printf("can read from the file\n");
    }
    while(1);
    return 0;
}
