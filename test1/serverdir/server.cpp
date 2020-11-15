#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "TcpServer.h"
using namespace std;

void sig_chld(int signo)
{
     pid_t pid;
     int stat;
     while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
          printf("child %d terminated\n",pid);
     return ;
}


int main(int argc,char **argv)
{
   TcpServer *TCP = new TcpServer;
   struct sockaddr_in addr,client_addr;
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(21);
   int listenfd = TCP->Socket(AF_INET, SOCK_STREAM, 0);   
   TCP->Bind(listenfd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));
   TCP->Listen(listenfd,ListenMax);

   void (TcpServer::*sig)(int) = &TcpServer::sig_chld;
   TCP->Signal(SIGCHLD, sig_chld);   
 
   for(;;)
   {
      socklen_t client_addr_len = sizeof(client_addr); 
      int clifd = TCP->Accept(listenfd,(struct sockaddr *)&client_addr,&client_addr_len);
      int clipid;
      if((clipid = fork()) == 0)
      {    
          close(listenfd);
          TCP->test_chat_function_2(clifd,"Register or Login?\n");
          TCP->test_chat_function_2(clifd,"Please input Account\n");
          TCP->test_chat_function_2(clifd,"Please input PassWord\n");
         
          

          close(clifd);
          exit(0);
      }
      else
      {
          close(clifd);          
      }
   }
   delete TCP;
   exit(0);
}
