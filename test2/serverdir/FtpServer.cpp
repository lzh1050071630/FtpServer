#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <string.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "FtpServer.h"


FtpServer::FtpServer()
{


}
FtpServer::~FtpServer()
{


}

void FtpServer::Ftp_Function(int client_fd)
{
    
     char RecvMsg[MAX_DATA];
     Ftp_Send_Msg(client_fd,"Please input FtpServer UseName\n");
     Ftp_Recv_Msg(client_fd,RecvMsg);
     fputs(RecvMsg,stdout);     
     bzero(RecvMsg,MAX_DATA); 
     
     Ftp_Send_Msg(client_fd,"Please input FtpServer PassWard\n");
     Ftp_Recv_Msg(client_fd,RecvMsg);
     fputs(RecvMsg,stdout);     
     bzero(RecvMsg,MAX_DATA); 
     
     Ftp_Send_Msg(client_fd,"Post or Passive?\n");
     Ftp_Recv_Msg(client_fd,RecvMsg);
     fputs(RecvMsg,stdout);    
     bzero(RecvMsg,MAX_DATA); 

}

void FtpServer::Ftp_Send_Msg(int client_fd,const char *SendMsg)
{
     char Send_buf[MAX_DATA];
     strcpy(Send_buf,SendMsg);
     write(client_fd,Send_buf,strlen(Send_buf));
     bzero(Send_buf,MAX_DATA);
}

void FtpServer::Ftp_Recv_Msg(int client_fd,char *RecvMsg)
{
     char Recv_buf[MAX_DATA];
     int n,inchars = 0;
     do
     {
          n = read(client_fd,&Recv_buf[inchars],MAX_DATA);
          if(n < 0)
             Ftp_err_sys("Read Error");
          inchars += n;        
     }while(Recv_buf[inchars-1]!='\n');
     strcpy(RecvMsg,Recv_buf);
}

void FtpServer::Ftp_err_sys(const char *str)
{
    perror(str);
    exit(1); 
} 


