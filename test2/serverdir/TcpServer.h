#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#define ListenMax 10
#define SEND_DATA_MAX 1024
typedef void Sigfunc(int);


class TcpServer
{

private:
      char Client_SEND_DATA[SEND_DATA_MAX][SEND_DATA_MAX];
public:
  TcpServer();
  ~TcpServer();
  int Socket(int domain, int type, int protocol);//创建套接字
  void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  void Connect(int socket, const struct sockaddr *addr, socklen_t addrlen);
  void Listen(int sockfd, int backing);
  int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
  ssize_t Write(int fd,const char *buf,size_t count);

  void getMsg(char **msgDate);

  void test_chat_function_1(int clifd);
  void test_chat_function_2(int clifd,const char *msg);
  Sigfunc *Signal(int signo, Sigfunc *func);
  static void sig_chld(int signo);
};
