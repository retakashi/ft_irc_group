#include "echoServer.hpp"

#define RCVBUFSIZE 32

void tcpClient(int clnt_sock)
{
    ssize_t recv_msg_size = 0;
    char buf[RCVBUFSIZE];
    if ((recv_msg_size = recv(clnt_sock, buf, RCVBUFSIZE, 0)) < 0)
        putError("recv failed");
    
    while(recv_msg_size > 0)
    {
        if (send(clnt_sock, buf, recv_msg_size, 0) < 0)
            putError("send failed");
        if ((recv_msg_size = recv(clnt_sock, buf, RCVBUFSIZE, 0)) < 0)
            putError("recv failed");
    }
    close (clnt_sock);
}

int main(int argc, char ** argv)
{
    int sock;
    struct  sockaddr_in serv_addr;
    unsigned short serv_port;
    char *serv_ip;
    char *echo_str;
    char buf[RCVBUFSIZE];
    unsigned int echo_str_len;
    int 
    return 0;
}
