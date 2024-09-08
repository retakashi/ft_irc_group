#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <iostream>
#include "echoServer.hpp"

// class for client
class Client
{
    private:
        // client file descriptor
        int sockfd; 
        // client ip address
        std::string IPadd; 
        bool auth();
    public:
        bool authenticated;
        std::string nickname;
        std::string username;
        std::string hostname;

        // Client(int sockfd) : socket(sock), authenticated(false) {}
        int GetFd(){return sockfd;}

        void SetFd(int fd){sockfd = fd;}
        void setIpAdd(std::string ipadd){IPadd = ipadd;}
};

#endif
