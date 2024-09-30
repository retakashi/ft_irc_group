#include "Server.hpp"

static std::string servrname_("servername");
static std::vector<ClientData> clients_;
static std::map<std::string, Channel *> channels_;
static void closeAllSocket();

Server::Server() {}
Server::Server(short port, std::string password)
    : port_(port), hostname_("hostname"), serverpass_(password) {}
Server::~Server() {}

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
    for (size_t i = 0; i < Server::clients_.size(); i++) {
      if (FD_ISSET(Server::clients_[i].getSocket(), &data.read_fds))
        handleClientCommunication(Server::clients_[i]);
    }
  }
  Server::closeAllSocket();
}

void Server::initServerSocket(struct sockaddr_in &sockaddr) {
  int on = 1;

  if ((serversock_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    putFunctionError("socket failed");
  if (setsockopt(serversock_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    putFunctionError("setsockopt failed");
  std::memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr.sin_port = htons(port_);
  if (bind(serversock_, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    putFunctionError("bind failed");
  if (listen(serversock_, SOMAXCONN) < 0) putFunctionError("listen failed");
  if (fcntl(serversock_, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
}

void Server::setSelectArgs(fd_set &read_fds, int &socket_max) {
  FD_ZERO(&read_fds);
  FD_SET(serversock_, &read_fds);
  socket_max = serversock_;
  for (size_t i = 0; i < Server::clients_.size(); i++) {
    FD_SET(Server::clients_[i].getSocket(), &read_fds);
    if (socket_max < Server::clients_[i].getSocket()) socket_max = Server::clients_[i].getSocket();
  }
}

void Server::closeAllSocket() {
  for (size_t i = 0; i < clients_.size(); i++) {
    if (close(clients_[i].getSocket()) < 0) perror("close failed");
  }
  if (close(serversock_) < 0) perror("close failed");
}

const std::string &Server::getHostname() const { return hostname_; }
