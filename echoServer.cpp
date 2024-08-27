#include "echoServer.hpp"

echoServer::echoServer(){}
echoServer::echoServer(unsigned short ports[]){buf[0]= '\0';}
echoServer::~echoServer(){}
echoServer::echoServer(const echoServer &other)
{}
echoServer &echoServer::operator=(const echoServer &other)
{
  if (this != &other)
  {
    size_t i = 0;
    for(i = 0;other.buf[i] != '\0';i++)
    {
      this->buf[i] = other.buf[i];
    }
    this->buf[i] = '\0';
  }
  return (*this);
}

void echoServer::startServer(unsigned short ports[]) {
  std::vector<int> serv_sockets;
  std::vector<struct sockaddr_in> serv_addr;
  std::vector<int> new_sockets;

  initServerSocket(ports, serv_sockets, serv_addr);

  fd_set read_fds, read_save, write_fds, write_save;
  struct timeval timeout;
  int sel_ret = 0;
  int new_sock = 0;

  FD_ZERO(&read_fds);
  FD_ZERO(&read_save);
  FD_ZERO(&write_fds);
  FD_ZERO(&write_save);
  for (int i = 0; i < PORT_MAX; i++) {
    FD_SET(serv_sockets[i], &read_save);
    FD_SET(serv_sockets[i], &write_save);
  }
  while (true) {
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    read_fds = read_save;
    write_fds = write_save;
    if ((sel_ret = select(FD_SETSIZE, &read_fds, &write_fds, NULL, &timeout)) <
        0)
      putError("select failed");
    // if (sel_ret == 0) {
    //   std::cout << "time out" << std::endl;
    //   break;
    // }
    if (new_sockets.size() > 0) {
      for (int i = 0; i < new_sockets.size(); i++) {
        if (FD_ISSET(new_sockets[i], &read_fds)) {
          FD_CLR(new_sockets[i], &read_fds);
          ft_recv(new_sockets[i]);
        }
        // if (FD_ISSET(new_sockets[i], &write_fds))
        // {
        //   FD_CLR(new_sockets[i],&write_fds);
        //   ft_send(new_sockets[i]);
        // }
      }
    }
    for (int i = 0; i < serv_sockets.size(); i++) {
      if (FD_ISSET(serv_sockets[i], &read_fds)) {
        FD_CLR(serv_sockets[i], &read_fds);
        if ((new_sock = accept(serv_sockets[i], 0, 0)) < 0)
          putError("accept");
        FD_SET(new_sock, &read_save);
        FD_SET(new_sock,&write_save);
        new_sockets.push_back(new_sock);
        ft_recv(new_sock);
      }
    }
  }
  for (int i = 0; i < serv_sockets.size(); i++) {
    if (close(serv_sockets[i]) < 0) putError("close failed");
  }
  for (int i = 0;i < new_sockets.size();i++)
  {
    if (close(new_sockets[i] < 0)) putError("close failed");
  }
}

void echoServer::initServerSocket(unsigned short ports[], std::vector<int> &serv_sockets,
                      std::vector<struct sockaddr_in> &serv_addr) {
  int sock = 0;
  struct sockaddr_in sock_addr;
  for (int i = 0; i < PORT_MAX; i++) {
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
      putError("socket failed");
    std::memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(ports[i]);
    serv_addr.push_back(sock_addr);
    serv_sockets.push_back(sock);
    if (bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
      putError("bind failed");
    if (listen(sock, MAX_PENDING) < 0) putError("listen failed");
  }
}

void echoServer::ft_recv(int sock) {
  int recv_size = 0;

  while (true) {
    recv_size = recv(sock, buf, RCVBUFSIZE, 0);
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      continue;
    else if (recv_size < 0)
      putError("recv failed");
    else
      break;
  }
  buf[recv_size] = '\0';
  std::cout << "received: " << buf << std::endl;
}

void echoServer::ft_send(int sock) {
  int send_ret = 0;
  while (true) {
    send_ret = send(sock, buf, RCVBUFSIZE, 0);
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      continue;
    else if (send_ret < 0)
      putError("send failed");
    else
      break;
  }
  // std::cout << "send: " << buf << std::endl;
}

void echoServer::putError(const char *errmsg) {
  perror(errmsg);
  throw std::exception();
}
