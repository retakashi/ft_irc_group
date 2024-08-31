#ifndef ECHOSERVER_HPP
#define ECHOSERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <fcntl.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>
#include <vector>

//ircのメッセージの長さは、最大で512文字（CR-LFを含む）
//（つまり、コマンドとそのパラメータに許される最大文字数は510文字。）文字列の後に"\r\n"がつく
#define RCVBUFSIZE 510

class echoServer
{
    private:
        char buf_[RCVBUFSIZE];
        short port_;
        echoServer();
        void initSocket(int &sock, struct sockaddr_in &sockaddr);
        void initSelectArgs(fd_set &read_fds,fd_set &write_fds,struct timeval &timeout);
        void setReadfds(int sock, std::vector<int> clients, fd_set &read_fds);
        int acceptNewClient(int sock,std::vector<int> &clients);
        void ft_recv(int sock);
        // void ft_send(int sock);
        void putError(const char *errmsg);
    public:
        echoServer(short port);
        ~echoServer();
        echoServer(const echoServer &other);
        echoServer &operator=(const echoServer &other);
        void startServer();      
};
#endif
