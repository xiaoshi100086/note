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
   printf("write size(%d) data:%s\n", wdsize, "helloworld");
   wdsize=sendto(sockfd,"helloworld",strlen("helloworld"),0,(struct sockaddr*)&svraddr,sizeof(svraddr));
   printf("write size(%d) data:%s\n", wdsize, "helloworld");
   wdsize=sendto(sockfd,"helloworld",strlen("helloworld"),0,(struct sockaddr*)&svraddr,sizeof(svraddr));
   printf("write size(%d) data:%s\n", wdsize, "helloworld");
   return 0;
}