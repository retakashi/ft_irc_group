#include "Server.hpp"

ssize_t Server::ft_send(ClientData client, size_t send_size) {
  ssize_t send_ret = 0;
  while (1) {
    send_ret = send(client.getSocket(), msg_, send_size, 0);
    if (send_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
      continue;
    else if (send_ret == 0) {
      msg_[0] = '\0';
      std::cout << "client send EOF..." << std::endl;
      disconnectClient(client);
      return 0;
    } else if (send_ret < 0)
      putFunctionError("send failed");
    else
      return send_size;
  }
}

size_t Server::strToCharArray(const std::string &casted_msg) {
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

int Server::sendCmdResponce(int code, const ClientData &client) {
  std::string resp_msg;
  size_t send_size = 0;

  resp_msg = createCmdRespMsg(servername_, code);
  send_size = strToCharArray(resp_msg);
  ft_send(client, send_size);
  return 0;
}

int Server::sendCmdResponce(int code, const std::string &str, const ClientData &client) {
  std::string resp_msg;
  size_t send_size = 0;

  resp_msg = createCmdRespMsg(servername_, code, str);
  send_size = strToCharArray(resp_msg);
  ft_send(client, send_size);
  return 0;
}

int Server::sendCmdResponce(int code, const std::string &str1, const std::string &str2,
                            const ClientData &client) {
  std::string resp_msg;
  size_t send_size = 0;

  resp_msg = createCmdRespMsg(servername_, code, str1, str2);
  send_size = strToCharArray(resp_msg);
  ft_send(client, send_size);
  return 0;
}
