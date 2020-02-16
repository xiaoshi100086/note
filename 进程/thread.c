// 代码说明

// 如下代码，运行如下进行编译
// gcc -o thread thread.c -lpthread

// 注意：不用担心僵尸进程的问题，因为pthread_create创建线程已经设置了CLONE_THREAD

#include <stdio.h>
#include <pthread.h>

void *myThread1(void) //线程函数
{
    int i;
    printf("pthread1 \n");
    return;
}

void *myThread2(void)
{
    int i;
    printf("pthread2 \n");
    return;
}

int main()
{
    int i=0,ret=0;
    pthread_t id1,id2;
    ret= pthread_create(&id1,NULL,(void*)myThread1,NULL ); //创建线程
    if(ret)
    {
        printf("create error\n");
        return 1;
    }
    ret = pthread_create(&id2,NULL,(void*)myThread2,NULL); //创建线程
    if(ret)
    {
        printf("create error\n");
        return 1;
    }
    
    pthread_join(id1,NULL); //当前线程会处于阻塞状态，直到被调用的线程结束后，当前线程才会重新开始执行
    pthread_join(id2,NULL);
    sleep(3);
    printf("main thread end\n");
    return 0;
}
