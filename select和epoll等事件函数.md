# 事件多路分发函数 select、poll、epoll

事件多路分发函数作用是监视socket事件，调用时会阻塞，除非是有socket事件到来或者等待超时。

## select

### 函数原型

```C
#include <sys/select.h>
#include <sys/time.h>
typedef struct
{
    #ifdef__USE_XOPEN
    __fd_maskfds_bits[__FD_SETSIZE/__NFDBITS]; //__FD_SETSIZE一般是1024，可以修改
    #define__FDS_BITS(set)((set)->fds_bits)
    #else
    __fd_mask__fds_bits[__FD_SETSIZE/__NFDBITS];
    #define__FDS_BITS(set)((set)->__fds_bits)
    #endif
}fd_set;


int select(int maxfdp1,fd_set *readset,fd_set *writeset,fd_set *exceptset,const struct timeval *timeout)
返回值：就绪描述符的数目，超时返回0，出错返回-1
```

### 使用流程
![select和epoll等事件函数_select原理](/assets/select和epoll等事件函数_select原理.PNG)

### 底层实现流程

1. 使用copy_from_user从用户空间拷贝fd_set到内核空间。
1. 注册回调函数__pollwait。
1. 遍历所有fd，依次调用对应的file.f_op->poll()方法，检测每个fd是否有io事件就绪。如果没有io事件，会在poll()中调用poll_wait()，把当前进程挂到fd的等待队列中。如果有io事件，poll()返回一个描述读写操作是否就绪的mask掩码，根据这个mask掩码给fd_set赋值。
1. 如果遍历完所有的fd，还没有io事件，则会调用schedule_timeout让当前进程进入睡眠状态。
1. 只要有事件触发，或者等待超时，系统调用返回，将fd_set从内核空间拷贝到用户空间，回到用户态。后面用户遍历fd_set，做对应的操作。

缺点：

1. fd_set实质上是位数组，长度由宏__FD_SETSIZE决定，默认是1024，这就决定了监听fd的最大值。当然你可以修改该宏，重新编译也是可以的，内核是没有相关长度限制的。
1. select调用涉及用户空间和内核空间的fd_set两次拷贝，并且拷贝按照监听fd的最大数量拷贝，而不是当前数量。无疑增加了成本。
1. 调用select后，还需要用户手动再次遍历fd_set。
1. 都是使用遍历方式，导致随着fd数增加，性能下降。

## poll

### 函数原型

```C
# include <poll.h>
struct pollfd {
    int fd;         /* 文件描述符 */
    short events;         /* 等待的事件 */
    short revents;       /* 实际发生了的事件 */
};

int poll ( struct pollfd * fds, unsigned int nfds, int timeout);
```
### 底层实现

poll实现和select相似，只不过fd集的结构不同，poll使用的是pollfd。同时poll使用的fd集并不是大小固定，而是由传进的参数nfds决定，解决了fd数上限限制。但还有select其他的缺点。


## epoll

### 函数原型

```C
#include <sys/epoll.h>
struct epoll_event {
  __uint32_t events;  /* Epoll events */
  epoll_data_t data;  /* User data variable */
};

int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
```

### 底层实现原理

1. epoll_create，在epoll文件系统建立了个file节点，并开辟epoll自己的内核高速cache区，建立红黑树，分配
好想要的size的内存对象，建立一个list链表，用于存储准备就绪的事件。其中size是建议遇分配空间值。
1. epoll_ctl，把要监听的socket放到对应的红黑树上，给内核中断处理程序注册一个回调函数，通知内核，如果
这个句柄的数据到了，就把它放到就绪列表。
1. epoll_wait，定时观察就绪列表里面有没有数据，并进行提取和清空就绪列表，非常高效。

### 优点：

1. select/poll是无状态的系统调用，所以每次调用都涉及用户空间与内存空间的拷贝。而epoll则使用内存映射的方式，并在epoll_create中创建epoll对象管理数据，所以无需拷贝。
1. epoll的fd集合是用红黑树来实现的，在epoll_create创建，由epoll_ctl增加节点。红黑树可以有效的识别重复添加fd的错误。select是用位数组来实现的，但缺点也比较明显。
1. 当fd有事件到来时，select/poll都是唤醒原来进程，而epoll则是调用回调函数，把fd对应的红黑树节点放到list链表中。不用再次遍历fd集合。
