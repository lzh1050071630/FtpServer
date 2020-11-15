#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <error.h>
#include <netinet/in.h> //定义 sockaddr_in结构体
#include <strings.h> //定义bzero()
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define MAXLINE 1024
#define DATAMAX 1024
using namespace std;


static int err_sys(const char *str)
{
    perror(str);
    exit(1);
}

//错误包裹函数
int Socket(int family,int type,int protocol)
{
    int n;
    if((n = socket(family,type,protocol))<0)
      err_sys("socket error");
    return n;
}

static void Ftp_Chat(int client_fd)
{
    
    char str[DATAMAX+1];
    int n;
    int inchars = 0;
    do
    {
         n = read(client_fd,&str[inchars],DATAMAX);
         if(-1 == n) 
         {
            err_sys("Read ERROR");
         }
         inchars += n;
         //std::cout<<"n = "<<n<<std::endl;
    }while(str[inchars-1]!='\n');//保证最后一个字符为\n,且为\n时，读取完毕
    str[inchars] = 0;//
    fputs(str,stdout);
    bzero(str,DATAMAX);
    fgets(str,DATAMAX,stdin);//自带\n
    write(client_fd,str,strlen(str));//注意这里用strlen(str)
    bzero(str,DATAMAX);
}



static void FtpClient_Function(int client_fd)
{
     char str[MAXLINE+1];
     int n;    
     while(fgets(str,sizeof(str),stdin))
     {
        str[MAXLINE] = '\0';
        if(write(client_fd,str,strlen(str))<0)
           err_sys("write error");
        bzero(str,MAXLINE);
        n = read(client_fd,str,MAXLINE);
        if(n < 0)
             err_sys("Read Error");
        if(fputs(str,stdout) == EOF)
               err_sys("fputs error");
        bzero(str,MAXLINE);
        if(n < 0) 
          err_sys("read error");
     }
}



int main(int argc,char **argv)
{
     int client_fd,n;
     struct sockaddr_in servaddr;

     if(argc<3)
        err_sys("usge:./client serverIp serverPost");

     if((client_fd = socket(AF_INET,SOCK_STREAM,0))<0)
        err_sys("create socket error!");
        
     bzero(&servaddr,sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(atoi(argv[2]));
     if(inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<=0)
        err_sys("inet_pton error");
     if(connect(client_fd,(const struct sockaddr *)&servaddr,sizeof(servaddr))<0)
        err_sys("connect error");
     Ftp_Chat(client_fd);//Register
     Ftp_Chat(client_fd);//Account
     Ftp_Chat(client_fd);//Password
     exit(0);
}
