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

#define MAX_PENDING 5
#define RCVBUFSIZE 10
#define PORT_MAX 2

class echoServer{
    private:
        char buf[RCVBUFSIZE];
        echoServer();
        void initServerSocket(unsigned short ports[], std::vector<int> &serv_sockets, std::vector<struct sockaddr_in> &serv_addr);
        void ft_recv(int sock);
        void ft_send(int sock);
        void putError(const char *errmsg);
    public:
        echoServer(unsigned short ports[]);
        ~echoServer();
        echoServer(const echoServer &other);
        echoServer &operator=(const echoServer &other);
        void startServer(unsigned short ports[]);
        
};
#endif
