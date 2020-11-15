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
#include "FtpServer.h"
using namespace std;


int main(int argc,char **argv)
{
   TcpServer *TCP = new TcpServer;
   FtpServer *FTP = new FtpServer;
   struct sockaddr_in addr,client_addr;
   addr.sin_family = AF_INET;
   addr.sin_addr.s_addr = htonl(INADDR_ANY);
   addr.sin_port = htons(21);
   int listenfd = TCP->Socket(AF_INET, SOCK_STREAM, 0);   
   TCP->Bind(listenfd, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));
   TCP->Listen(listenfd,ListenMax);

   //Handling zombie processes
   TCP->Signal(SIGCHLD, TcpServer::sig_chld);   
 
   for(;;)
   {
      socklen_t client_addr_len = sizeof(client_addr); 
      int clifd = TCP->Accept(listenfd,(struct sockaddr *)&client_addr,&client_addr_len);
      int clipid;
      if((clipid = fork()) == 0)
      {    
          close(listenfd);
          FTP->Ftp_Function(clifd);
          close(clifd);
          delete TCP;
          delete FTP;
          exit(0);
      }
      else
      {
          close(clifd);          
      }
   }
   delete TCP;
   delete FTP;
   exit(0);
}
