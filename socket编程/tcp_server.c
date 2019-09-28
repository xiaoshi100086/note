#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>
#include <arpa/inet.h>

//关于服务器的socket
void error_exit(char *name)
{
   perror(name);
   exit(-1);
}
int main(int argc,char *argv[])
{
   // AF_INET  表示使用ipv4协议进行通信
   // AF_INET6 表示使用ipv6协议进行通信
   // SOCK_STREAM 流式套接字，也就是tcp协议，数据是顺序发送的，也是顺序接受的。同时上一包数据与下一包数据没有明显界限
   // SOCK__DGRAM 数据包套接字，也就是udp协议，数据是打包成数据包发送的，接受的数据可能与发送的数据顺序不一致，接受的数据也可能出错。
   int sockfd=socket(AF_INET,SOCK_STREAM,0);

   if(sockfd<0)
   {
        error_exit("create error");
   }
   //绑定地址(ip和端口号)
   struct sockaddr_in svraddr;
   memset(&svraddr,0,sizeof(svraddr));
   svraddr.sin_family=AF_INET;
   svraddr.sin_addr.s_addr=INADDR_ANY; // INADDR_ANY 表示 0.0.0.0
   //svraddr.sin_addr.s_addr=inet_addr("127.0.0.1"); // 第二种写法
   svraddr.sin_port=htons(5555); // 大于1024，系统用的都是小于1024
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
            memset(buf, 0, sizeof(buf));
            rdsize =read(fd,buf,4);
            printf("read from ip(%s) port(%u) rdsize(%d) data:%s\n", inet_ntoa(removeaddr.sin_addr), removeaddr.sin_port, rdsize, buf);
        }
    }
    return 0;
}