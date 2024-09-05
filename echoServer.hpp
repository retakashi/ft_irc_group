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
#include <climits>
#include <cstring>
#include <cstdio>

//ircのメッセージの長さは、最大で512文字（CR-LFを含む）
//（つまり、コマンドとそのパラメータに許される最大文字数は510文字。）文字列の後に"\r\n"がつく
#define RCVBUFSIZE 510

static bool g_sig_flg = false;
//メンバー変数は最後に_を付けてます
class echoServer{
    private:
        short port_;
        //クライアントのsocket
        std::vector<int> clients_;
        char msg_[RCVBUFSIZE];
        echoServer();
        void initSocket(int &sock, struct sockaddr_in &sockaddr);
        void initSelectArgs(fd_set &read_fds,fd_set &write_fds,struct timeval &timeout);
        void setReadfds(int sock, fd_set &read_fds);
        void acceptNewClient(int sock);
        void ft_recv(size_t i);
        void ft_send(size_t i, size_t send_size);
        void disconnectClient(size_t i);
    public:
        echoServer(short port);
        ~echoServer();
        echoServer(const echoServer &other);
        echoServer &operator=(const echoServer &other);
        void startServer();      
};
void putError(const char *errmsg);
#endif
