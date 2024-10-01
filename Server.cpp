#include "Server.hpp"

std::string Server::servername_("servername");
int Server::serversock_ = 0;
std::vector<ClientData> Server::clients_;
std::map<std::string, Channel *> Server::channels_;

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
  for (size_t i = 0; i < Server::clients_.size(); i++) {
    FD_SET(Server::clients_[i].getSocket(), &read_fds);
    if (socket_max < Server::clients_[i].getSocket()) socket_max = Server::clients_[i].getSocket();
  }
}

void Server::closeAllSocket() {
  for (size_t i = 0; i < clients_.size(); i++) {
    if (close(clients_[i].getSocket()) < 0) perror("close failed");
  }
  if (serversock_ != 0 && close(serversock_) < 0) perror("close failed");
}

const std::string &Server::getHostname() const { return hostname_; }

// void Server::sendMessage(ClientData& client, const std::string& message) {
//     std::string formattedMessage = message + "\r\n";
//     ft_send(client, formattedMessage.c_str(), formattedMessage.size());
// }

Channel* Server::getChannelByName(const std::string& name) {
    std::map<std::string, Channel*>::iterator it = channels_.find(name);
    if (it != channels_.end()) {
        return it->second;
    }
    return nullptr;
}

void Server::addChannel(const std::string& name, Channel* channel) {
    channels_.insert(std::make_pair(name, channel));
}

ClientData* Server::getClientByNickname(const std::string& nickname) {
    for (std::vector<ClientData>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        if (it->getNickname() == nickname) {
            return &(*it);
        }
    }
    return nullptr;
}

void Server::addClient(const ClientData& client) {
    clients_.push_back(client);
}

void Server::removeClient(const std::string& nickname) {
    clients_.erase(std::remove_if(clients_.begin(), clients_.end(),
                                  [&nickname](const ClientData& client) {
                                      return client.getNickname() == nickname;
                                  }),
                   clients_.end());
}

void Server::handleJoin(const std::string& channelName, ClientData& client) {
    Channel* channel = getChannelByName(channelName);
    if (!channel) {
        channel = new Channel(channelName);
        addChannel(channelName, channel);
    }

    channel->addClient(&client);

    // クライアントにJOINメッセージを送信
    std::string joinMsg = ":" + getServername() + " JOIN " + channelName + "\r\n";
    ft_send(client, joinMsg.size());

    // チャンネルのトピックを確認し、適切なレスポンスコードを送信
    if (channel->getTopic().empty()) {
        std::string noTopicMsg = ":" + getServername() + " 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n";
        ft_send(client, noTopicMsg.size());
    } else {
        std::string topicMsg = ":" + getServername() + " 332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        ft_send(client, topicMsg.size());
    }
}
