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
#include <sys/wait.h>
#include "TcpServer.h"



static void err_sys(const char *str)
{
    perror(str);
    exit(1);
}


TcpServer::TcpServer()
{

}

TcpServer::~TcpServer()
{

}


//创建套接字
int TcpServer::Socket(int domain,int type, int protocol)
{
    int n;
    if((n = socket(domain,type,protocol))<0)
       err_sys("Create Socket Error");
    return n;  
}




void TcpServer::Bind(int sockfd,const struct sockaddr *addr,socklen_t addrlen)
{
    int n;
    if((n = bind(sockfd,addr,addrlen))<0)
      err_sys("Bind Socket Error");
}

void TcpServer::Connect(int socket,const struct sockaddr *addr,socklen_t addrlen)
{
    if(connect(socket,addr,addrlen)<0) 
      err_sys("Connect Error");
       
}
void TcpServer::Listen(int sockfd,int backing)
{
    if(listen(sockfd,backing)<0)
      err_sys("Listen Error");
}
int TcpServer::Accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{  
    int n;
    if((n = accept(sockfd,addr,addrlen))<0)
       err_sys("Accept Error");
    return n;
}

ssize_t TcpServer::Write(int fd,const char *buf,size_t count)
{
    ssize_t n;
    char send_data[SEND_DATA_MAX];
    snprintf(send_data,sizeof(send_data),"%s\r\n",buf);
    if(write(fd,send_data,sizeof(send_data))<0)
       err_sys("Write ERROR");
    return n; 
}




Sigfunc * TcpServer::Signal(int signo, Sigfunc *func)
{    struct sigaction act, oact;
     act.sa_handler = func;
     sigemptyset(&act.sa_mask);
     act.sa_flags = 0;
     if(signo == SIGALRM){
#ifdef SA_INTERRUPT
       act.sa_flags |= SA_INTERRUPT;
#endif
     }else{
#ifdef SA_RESTART
       act.sa_flags |= SA_RESTART;
#endif
     }
     if(sigaction(signo,&act,&oact) < 0)
       return(SIG_ERR);
     return(oact.sa_handler);
}

void TcpServer::sig_chld(int signo)
{
     pid_t pid;
     int stat;
     while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
          printf("child %d terminated\n",pid);
     return ;
}




//测试于客户端交互模块 版本一
void TcpServer::test_chat_function_1(int clifd)
{
     char buf[SEND_DATA_MAX];
     int n;
     while((n = read(clifd,buf,sizeof(buf))) > 0)
     {
          
          fputs(buf,stdout);
          bzero(buf,SEND_DATA_MAX);
          fgets(buf,SEND_DATA_MAX,stdin);
          write(clifd,buf,strlen(buf));
          bzero(buf,SEND_DATA_MAX);        
     }
}

//测试于客户端进行交互模块 版本二
void TcpServer::test_chat_function_2(int clifd,const char *msg)
{
     char buf[SEND_DATA_MAX+1];
     int n, inchars = 0;    
     strcpy(buf,msg); 
     write(clifd,buf,strlen(buf));
     bzero(buf,SEND_DATA_MAX);
     do
     {
          n = read(clifd,&buf[inchars],SEND_DATA_MAX);
          if(-1 == n)
            err_sys("Read ERROR");
          inchars += n;
          //std::cout<<n<<std::endl;
     }while(buf[inchars-1]!='\n');     
     fputs(buf,stdout);

     //fetch information
     char client_msg[SEND_DATA_MAX+1];
     int buf_len = strlen(buf)-1;
     strncpy(client_msg,buf,buf_len);
     client_msg[buf_len] = '\0';  
     if(!strcmp(client_msg,"lzh"))
     {
       int fd = open(client_msg,O_WRONLY|O_CREAT,0644);
       close(fd);
     }
     
     bzero(buf,SEND_DATA_MAX);
     bzero(client_msg,SEND_DATA_MAX+1);
}


