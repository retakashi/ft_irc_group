#include "Server.hpp"

Server::Server() {}
Server::Server(short port) : port_(port) { std::memset(msg_, 0, sizeof(msg_)); }
Server::~Server() {}
Server::Server(const Server &other) { *this = other; }
Server &Server::operator=(const Server &other) {
  if (this != &other) {
    port_ = other.port_;
    socket_ = other.socket_;
    size_t i = 0;
    for (i = 0; other.msg_[i] != '\0'; i++) this->msg_[i] = other.msg_[i];
    this->msg_[i] = '\0';
    this->clients_ = other.clients_;
  }
  return (*this);
}

void Server::startServer() {
  struct sockaddr_in sockaddr;
  fd_set read_fds;
  int max_sock = 0;
  struct timeval timeout;
  int sel_ret = 0;
  int client_sock = 0;

  // listenできるところまでsocketを設定
  initServerSocket(sockaddr);
  initSelectArgs(read_fds, max_sock, timeout);
  while (1) {
    setReadfds(read_fds);
    max_sock = getMaxSocket();
    sel_ret = select(max_sock + 1, &read_fds, NULL, NULL, &timeout);
    if (sel_ret < 0) putFunctionError("select failed");
    if (sel_ret == 0) {
      std::cout << "Time out" << std::endl;
      break;
    }
    if (FD_ISSET(socket_, &read_fds)) {
      client_sock = acceptNewClient();
      authenticatedNewClient(client_sock);
    }
    //   for (size_t i = 0; i < clients_.size(); i++) {
    //     if (FD_ISSET(clients_[i].getSocket(), &read_fds)) {
    //       FD_CLR(clients_[i].getSocket(), &read_fds);
    //       ft_recv(clients_[i].getSocket());
    //     }
    //   }
  }
}

void Server::initServerSocket(struct sockaddr_in &sockaddr) {
  int on = 1;

  if ((socket_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) putFunctionError("socket failed");
  if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    putFunctionError("setsockopt failed");
  std::memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr.sin_port = htons(port_);
  if (bind(socket_, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    putFunctionError("bind failed");
  if (listen(socket_, SOMAXCONN) < 0) putFunctionError("listen failed");
  if (fcntl(socket_, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
}

void Server::initSelectArgs(fd_set &read_fds, int &socket_max, timeval &timeout) {
  FD_ZERO(&read_fds);
  socket_max = getMaxSocket();
  timeout.tv_sec = 5000;
  timeout.tv_usec = 0;
}

void Server::setReadfds(fd_set &read_fds) {
  FD_SET(socket_, &read_fds);
  for (size_t i = 0; i < clients_.size(); i++) FD_SET(clients_[i].getSocket(), &read_fds);
}

int Server::getMaxSocket() {
  int max = 0;
  if (clients_.size() == 0) return socket_;
  for (size_t i = 0; i < clients_.size(); i++) {
    if (max < clients_[i].getSocket()) max = clients_[i].getSocket();
  }
  return max;
}

int Server::acceptNewClient() {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int new_client_sock = 0;

  if ((new_client_sock = accept(socket_, (struct sockaddr *)&client_addr, &addr_len)) < 0)
    putFunctionError("accept failed");
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
  std::cout << "connected sockfd: " << new_client_sock << std::endl;
  return (new_client_sock);
}

/* PASSも追加する
戻り値は特に意味なし */
int Server::authenticatedNewClient(int client_sock) {
  std::string command;
  std::string param;
  std::string casted_msg;
  std::string::size_type pos = 0;
  ClientData new_client(client_sock);

  while (new_client.isCompleteUserParams() == false) {
    ft_recv(client_sock);
    casted_msg = msg_;
    pos = splitCommand(casted_msg, command);
    if (command != "NICK" && command != "USER"&&command!= "PASS") return printCmdResponce(421, new_client, command);
    if (pos == std::string::npos) return printCmdResponce(461, new_client, command);
    splitParam(casted_msg, param, pos);
    if (command == "PASS" && param != pass_)
    {
      std::cout << param << pass_ << std::endl;
      return printCmdResponce(461, new_client, command);
    }
    if (command == "NICK") {
      if (isValidNickname(param) == false)
        return printCmdResponce(421, new_client,param);
      else
        new_client.setNickname(param);
    } else if (command == "USER") {
      if (new_client.isUserParamCountValid(param) == false)
        return printCmdResponce(421, new_client, param);
      else
        new_client.setUserParams(param);
    }
  }
  clients_.push_back(new_client);
  return printWelcomeToIrc(new_client);
}

void Server::ft_recv(int socket) {
  int recv_size = 0;

  while (1) {
    recv_size = recv(socket, msg_, MAX_BUFSIZE, 0);
    if (recv_size < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
      continue;
    else if (recv_size <= 0 && errno == ECONNRESET) {
      disconnectClient(socket);
      return;
    } else if (recv_size < 0)
      putFunctionError("recv failed");
    else
      break;
  }
  msg_[recv_size - 1] = '\0';
}

void Server::disconnectClient(ClientData client) {
  std::vector<ClientData>::iterator it = clients_.begin();
  std::vector<ClientData>::iterator erase_it = it;
  while (it != clients_.end()) {
    if (it->getSocket() == client.getSocket()) break;
    it++;
  }
  std::cout << "disconnected sockfd : " << client.getSocket() << std::endl;
  if (close(it->getSocket()) < 0) putFunctionError("close failed");
  clients_.erase(it);
}

std::string::size_type Server::splitCommand(std::string casted_msg, std::string &command) {
  std::string::size_type pos = casted_msg.find(" ");
  if (pos != std::string::npos) {
    command = casted_msg.substr(0, pos);
    command[pos] = '\0';
  }
  return pos;
}

void Server::splitParam(std::string casted_msg, std::string &param, std::string::size_type pos) {
  size_t i = 0;

  casted_msg = casted_msg.substr(pos + 1);
  while (casted_msg[i] == ' ') i++;
  param = casted_msg.substr(i);
}

void Server::ft_send(ClientData client, size_t send_size) {
  int send_ret = 0;

  while (1) {
    send_ret = send(client.getSocket(), msg_, send_size, 0);
    if (send_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
      continue;
    else if (send_ret < 0 && errno == ECONNRESET) {
      msg_[0] = '\0';
      disconnectClient(client);
      return;
    } else if (send_ret < 0)
      putFunctionError("send failed");
    else
      break;
  }
}

size_t Server::createSendMsg(const std::string &casted_msg) {
  std::memset(msg_, 0, sizeof(msg_));
  size_t i = 0;
  while (casted_msg[i] != '\0' && i < MAX_BUFSIZE) {
    msg_[i] = casted_msg[i];
    i++;
  }
  msg_[i] = '\r';
  msg_[i + 1] = '\n';
  return i + 2;
}

int Server::printWelcomeToIrc(const ClientData &client) {
  std::stringstream ss;
  size_t send_size = 0;
  ss << ":001 Welcome to the Internet Relay Network " << client.getNickname() << "!"
     << client.getUsername() << "@" << client.getHostname();
  send_size = createSendMsg(ss.str());
  ft_send(client, send_size);
  return 0;
}

int Server::printCmdResponce(int code, const std::string &str) {
  std::cout << code << str << std::endl;
  
  return (0);  // false
}

int Server::printCmdResponce(int code, const ClientData &client, const std::string &str) {
  std::stringstream ss;
  size_t send_size = 0;
  switch (code) {
    case 421:
      ss << str << " :Unknown command";
      break;
    case 431:
      ss << ":No nickname given";
      break;
    case 432:
      ss << str << " :Erroneous nickname";
      break;
    case 433:
      ss << str << " :Nickname is already in use";
      break;
    case 461:
      ss << str << " :Not enough parameters";
      break;
  }
  send_size = createSendMsg(ss.str());
  ft_send(client, send_size);
  return (0);  // false
}


void Server::handleJoinCommand(ClientData* client, const std::string& channelName) {
    // チャンネルが存在しない場合は作成
    if (channels_.find(channelName) == channels_.end()) {
        channels_[channelName] = Channel(channelName);
    }
    
    // クライアントをチャンネルに追加
    channels_[channelName].addClient(client);

    // クライアントにJOINメッセージを送信
    std::string joinMessage = ":" + client->getNickname() + " JOIN :" + channelName + "\r\n";
    channels_[channelName].broadcastMessage(joinMessage, client);  // チャンネルの全員に通知

    // クライアントにトピックを送信
    std::string topic = channels_[channelName].getTopic();
    std::string topicMessage = ":server 332 " + client->getNickname() + " " + channelName + " :" + topic + "\r\n";
    client->sendMessage(topicMessage);

    // クライアントにユーザーリストを送信 (RPL_NAMREPLY)
    std::string nameReply = ":server 353 " + client->getNickname() + " = " + channelName + " :";
    const std::vector<ClientData*>& clients = channels_[channelName].getClients();  // クライアントのリストを取得
    for (std::vector<ClientData*>::const_iterator it = clients.begin(); it != clients.end(); ++it) {
        ClientData* member = *it;
        nameReply += member->getNickname() + " ";
    }

    nameReply += "\r\n";
    client->sendMessage(nameReply);

    // ユーザーリストの終了を通知 (RPL_ENDOFNAMES)
    std::string endOfNames = ":server 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list\r\n";
    client->sendMessage(endOfNames);
}

