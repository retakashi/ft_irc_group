#include "Server.hpp"

std::string Server::servername_("ft_irc");
int Server::serversock_ = 0;
std::list<ClientData> Server::clients_;
std::map<std::string, Channel *> Server::channels_;

Server::Server() {}
Server::Server(short port, std::string password)
    : port_(port), hostname_("localhost"), serverpass_(password) {}
Server::~Server() {
  //仮
  for (std::map<std::string, Channel *>::iterator it = Server::channels_.begin(); it != Server::channels_.end();it++)
  {
    delete it->second;
  }
}

void Server::startServer() {
  startserv_data data;

  // listenできるところまでsocketを設定
  initServerSocket(data.sockaddr);
  data.timeout.tv_sec = 400;
  data.timeout.tv_usec = 0;
  while (true) {
    setSelectArgs(data.read_fds, data.max_sock);
    data.sel_ret = select(data.max_sock + 1, &data.read_fds, NULL, NULL, &data.timeout);
    if (data.sel_ret < 0) putFunctionError("select failed");
    if (data.sel_ret == 0) {
      std::cout << "Time out" << std::endl;
      break;
    }
    if (FD_ISSET(serversock_, &data.read_fds)) data.client_sock = acceptNewClient();
    for (std::list<ClientData>::iterator it = Server::clients_.begin(); it != Server::clients_.end();it++) {
      if (it->getSocket() == -1)
      {
        std::list<ClientData>::iterator erase_it = it;
        it++;
        clients_.erase(erase_it);
      }
      if (it != Server::clients_.end() && FD_ISSET(it->getSocket(), &data.read_fds))
        handleClientCommunication(*it);
    }
  }
  Server::closeAllSocket();
}

void Server::initServerSocket(struct sockaddr_in &sockaddr) {
  int on = 1;

  if ((Server::serversock_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    putFunctionError("socket failed");
  if (setsockopt(Server::serversock_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    putFunctionError("setsockopt failed");
  std::memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr.sin_port = htons(port_);
  if (bind(Server::serversock_, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    putFunctionError("bind failed");
  if (listen(Server::serversock_, SOMAXCONN) < 0) putFunctionError("listen failed");
  if (fcntl(Server::serversock_, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
}

void Server::setSelectArgs(fd_set &read_fds, int &socket_max) {
  FD_ZERO(&read_fds);
  FD_SET(Server::serversock_, &read_fds);
  socket_max = Server::serversock_;
  for (std::list<ClientData>::iterator it = Server::clients_.begin(); it != Server::clients_.end();it++) {
    if (it->getSocket() != -1)
    FD_SET(it->getSocket(), &read_fds);
    if (socket_max < it->getSocket()) socket_max = it-> getSocket();
  }
}

void Server::closeAllSocket() {
  for (std::list<ClientData>::iterator it = Server::clients_.begin();it != Server::clients_.end();it++) {
    if (close(it->getSocket()) < 0) perror("close failed");
  }
  if (serversock_ != 0 && close(serversock_) < 0) perror("close failed");
}

const std::string &Server::getHostname() const { return hostname_; }
