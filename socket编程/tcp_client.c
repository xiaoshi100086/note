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

   char *send_msg = "helloworld";
   char print_buffer[16];
   size_t len = strlen(send_msg);
   while(len>0)
   {
      memset(print_buffer, 0, sizeof(print_buffer));
      int wdsize=write(fd,send_msg,2);
      memcpy(print_buffer, send_msg, wdsize);
      printf("write size(%d) data:%s\n", wdsize, print_buffer);
      send_msg+=wdsize;
      len-=wdsize;
   }
   return 0;
}