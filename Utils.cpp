#include "Server.hpp"

void Server::handleClientCommunication(ClientData &client) {
  if (client.isCompleteAuthParams() == false)
    authenticatedNewClient(client);
  else  // チャンネルとか認証以外はここ
    handleCommands(client);
}


// eraseしたイテレーターを参照しないか確認する！！！
void Server::disconnectClient(ClientData client) {
  int socket = client.getSocket();
  if (clients_.size() == 0) {
    if (close(socket) < 0) putFunctionError("close failed");
  } else {
    std::vector<ClientData>::iterator it = clients_.begin();
    std::vector<ClientData>::iterator erase_it = it;
    while (it != clients_.end()) {
      if (it->getSocket() == socket) break;
      it++;
    }
    if (close(socket) < 0) putFunctionError("close failed");
    clients_.erase(it);
  }
  std::cout << "disconnected sockfd : " << socket << std::endl;
}

void Server::splitCmdAndParam(std::string casted_msg, std::string &command, std::string &param) {
  std::string::size_type pos = casted_msg.find(" ");
  if (pos != std::string::npos) {
    command = casted_msg.substr(0, pos);
    command[pos] = '\0';
    param = casted_msg.substr(pos + 1);
  } else {
    command = casted_msg;
    param.clear();
  }
}


void Server::putFunctionError(const char *errmsg) {
  perror(errmsg);
  closeAllSocket();
  throw std::exception();
}