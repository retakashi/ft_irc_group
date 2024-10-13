#include "Server.hpp"

void Server::ft_send(std::string msg, ClientData client) {
  std::cout << "send: " << msg << std::endl;
  char casted_msg[MAX_BUFSIZE];
  ssize_t send_ret = 0;
  size_t send_size = 0;
  memset(casted_msg, 0, sizeof(casted_msg));
  // msgをsendに使用できる形にする
  if (msg.size() > MAX_BUFSIZE - 2)
    msg.resize(MAX_BUFSIZE - 2);
  msg += "\r\n";
  send_size = msg.size();
  std::strncpy(casted_msg, msg.c_str(), send_size);
  send_ret = send(client.getSocket(), casted_msg, send_size, 0);
  if (send_ret == 0) {
    std::cout << "client send EOF..." << std::endl;
    disconnectClient(client);
  }
  if (send_ret < 0) perror("send failed");
}

int Server::sendCmdResponce(int code, const ClientData client) {
  ft_send(createCmdRespMsg(servername_, client.getNickname(), code), client);
  return 0;
}

int Server::sendCmdResponce(int code, const std::string& str, ClientData client) {
  ft_send(createCmdRespMsg(servername_, client.getNickname(), code, str), client);
  return 0;
}

int Server::sendCmdResponce(int code, const std::string& str1, const std::string& str2,
                            ClientData client) {
  ft_send(createCmdRespMsg(servername_, client.getNickname(), code, str1, str2), client);
  return 0;
}

void Channel::broadcastMessage(const std::string& message, ClientData* sender) {
  for (std::vector<ClientData*>::iterator it = members_.begin(); it != members_.end(); ++it) {
    if (*it != sender) {
      Server::ft_send(message, **it);
    }
  }
  for (std::vector<ClientData*>::iterator it = operators_.begin(); it != operators_.end(); ++it) {
    if (*it != sender) {
      Server::ft_send(message, **it);
    }
  }
}

void Channel::sendAll(const std::string& message) {
  for (std::vector<ClientData*>::iterator it = members_.begin(); it != members_.end(); ++it) {
    Server::ft_send(message, **it);
  }
  for (std::vector<ClientData*>::iterator it = operators_.begin(); it != operators_.end(); ++it) {
    Server::ft_send(message, **it);
  }
}
