#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h> /* for struct sockaddr_in*/
#include <sys/errno.h>
#include <signal.h>
#include <arpa/inet.h>

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
         printf("read from ip(%s) port(%u) rdsize(%d) data:%s\n", inet_ntoa(removeaddr.sin_addr), removeaddr.sin_port, rdsize, buf);
      }
   }
   return 0;
}