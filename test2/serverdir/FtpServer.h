#include<string>

#define MAX_DATA 1024

class FtpServer
{
private:
	
public:
       FtpServer();
       ~FtpServer();

       void Ftp_Function(int client_fd);
       void Ftp_Send_Msg(int client_fd, const char *SendMsg);
       void Ftp_Recv_Msg(int client_fd, char *RecvMsg);
       void Ftp_Port();
       void Ftp_Passive();
       void Ftp_err_sys(const char *str);
    
};


