# socket

socket编程分为tcp和udp。socket初始化时，没有明显tcp和udp之分，根据使用的函数来加以区别。

# tcp

tcp有服务端和客户端之分。服务端需要调用bind、listen来监听，客户端需要调用connect主动连接，随后服务端调用accept接受连接。
tcp是面向连接的，所以一开始需要客户端的connect和服务端的accept来建立连接。
tcp是面向流的，连接建立好后，可以认为在客户端与服务端之间有个管道，管道两侧有读写缓冲区。写进去的数据流可能组合成缓冲区大小进行发送，接受的是一串字节流。所以TCP会有粘包问题。

服务端代码

```C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>

//关于服务器的socket
void error_exit(char *name)
{
   perror(name);
   exit(-1);
}
int main(int argc,char *argv[])
{
   int sockfd=socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
   {
        error_exit("create error");
   }
   //绑定地址(ip和端口号)
   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr=INADDR_ANY;
   //svraddr.sin_addr.s_addr=inet_addr("127.0.0.1");//第二种写法
   svraddr.sin_port=htons(5555);//大于1024，系统用的都是小于1024
   int ret=bind(sockfd,(struct sockaddr*)&svraddr,sizeof(svraddr));
    if(ret<0)
    {
        error_exit("bind error");
    }
   //设置监听参数back login,半连接数的最大
   ret=listen(sockfd,1024);
   if(ret<0)
   {
       error_exit("listen error");
   }

   struct sockaddr_in removeaddr;
   int addr_len=sizeof(removeaddr);//一定要是长度，不能为0
   while(1)
   {
        //返回一个和客户端通信的文件描述符
        int fd=accept(sockfd,(struct sockaddr*)&removeaddr,&addr_len);
        //TCP读写规则
        //1.读的时候没有数据，进去阻塞
        //2.如果关闭，读取返回0
        //3.如果关闭，写入数据会产生SIGPIPE

        char buf[1024]={0};
        int rdsize = -1;
        while(rdsize != 0)
        {
            rdsize =read(fd,buf,3);
            printf("read size %d,data=%s\n",rdsize,buf);
        }
    }
    return 0;
}
```

客户端代码

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>

//关于客户端的socket
void error_exit(char *name)
{
   perror(name);
   exit(-1);
}
int main(int argc,char *argv[])
{
   if(argc<3)
   {
        printf("run program+ip+port\n");
        return-1;
   }
   int fd=socket(AF_INET,SOCK_STREAM,0);
   if(fd<0)
   {
        error_exit("create error");
   }
   //连接服务器，设置服务器的地址(ip和端口)
   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr= inet_addr(argv[1]);
   svraddr.sin_port=htons(atoi(argv[2]));
   int ret =connect(fd,(struct sockaddr *)&svraddr,sizeof(svraddr));
   if(ret<0)
   {
      error_exit("connect error");
   }

   char *hello = "helloworld";
   size_t len = strlen(hello);
   while(len>0)
   {
      int wdsize=write(fd,hello,2);
      printf("write size=%d\n",wdsize);
      hello+=wdsize;
      len-=wdsize;
   }
   return 0;
}
```

# udp

upd是没有服务端和客户端之分，也无需建立连接。socket建立后就可以使用，主动方使用sendto写入报文，被动方使用recvfrom读取报文，代码比较简洁。
upd是面向报文包的，所以没有tcp的粘包现象。

被动方代码

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>

//关于服务器udp的socket
void error_exit(char *name)
{
   perror(name);
   exit(-1);
}
int main(int argc,char *argv[])
{
   int sockfd=socket(AF_INET,SOCK_DGRAM,0);
   if(sockfd<0)
   {
        error_exit("create error");
   }
   //绑定地址(ip和端口号)
   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr=INADDR_ANY;
   //svraddr.sin_addr.s_addr=inet_addr("127.0.0.1");第二种写法
   svraddr.sin_port=htons(5555);
   int ret=bind(sockfd,(struct sockaddr*)&svraddr,sizeof(svraddr));
   if(ret<0)
   {
      error_exit("bind error");
   }
   char buf[1024]={0};
   struct sockaddr_in removeaddr;
   int addrlen=sizeof(removeaddr);
   while(1)
   {
       memset(buf,0,1024);
       int rdsize= recvfrom(sockfd,buf,1024,0,(struct sockaddr*)&removeaddr,&addrlen);
       if(rdsize>0)
       {
            printf("read data %s\n",buf);
       }
   }
   return 0;
}
```

主动端代码

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>


//关于客户端udp的socket
void error_exit(char *name)
{
   perror(name);
   exit(-1);
}
int main(int argc,char *argv[])
{
   int sockfd=socket(AF_INET,SOCK_DGRAM,0);
   if(sockfd<0)
   {
        error_exit("create error");
   }
   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
   svraddr.sin_port= htons(5555);
   svraddr.sin_family=AF_INET;
   int wdsize=sendto(sockfd,"helloworld",strlen("helloworld"),0,(struct sockaddr*)&svraddr,sizeof(svraddr));
   printf("write size%d\n",wdsize);
   wdsize=sendto(sockfd,"helloworld",strlen("helloworld"),0,(struct sockaddr*)&svraddr,sizeof(svraddr));
   printf("write size%d\n",wdsize);
   wdsize=sendto(sockfd,"helloworld",strlen("helloworld"),0,(struct sockaddr*)&svraddr,sizeof(svraddr));
   printf("write size%d\n",wdsize);
   return 0;
}
```