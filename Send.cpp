#include "Server.hpp"

void Server::ft_send(std::string msg, ClientData client) {
  char casted_msg[MAX_BUFSIZE];
  ssize_t send_ret = 0;
  size_t send_size = msg.size();

  std::cout << client.getSocket() << std::endl;
  // msgをsendに使用できる形にする
  if (send_size < MAX_BUFSIZE - 2) msg.resize(MAX_BUFSIZE - 2);
  std::strncpy(casted_msg, msg.c_str(), send_size);
  casted_msg[send_size] = '\r';
  casted_msg[send_size + 1] = '\n';

  send_ret = send(client.getSocket(), casted_msg, send_size + 2, 0);
  if (send_ret == 0) {
    std::cout << "client send EOF..." << std::endl;
    disconnectClient(client);
  }
  if (send_ret < 0)
    perror("send failed");
}

int Server::sendCmdResponce(int code, const ClientData client) {
  ft_send(createCmdRespMsg(servername_, code), client);
  return 0;
}

int Server::sendCmdResponce(int code, const std::string &str, ClientData client) {
  ft_send(createCmdRespMsg(servername_, code, str), client);
  return 0;
}

int Server::sendCmdResponce(int code, const std::string &str1, const std::string &str2,
                            ClientData client) {
  ft_send(createCmdRespMsg(servername_, code, str1, str2), client);
  return 0;
}
