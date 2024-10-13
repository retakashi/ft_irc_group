#include "Server.hpp"

void Server::handleClientCommunication(ClientData &client) {
  if (client.isCompleteAuthParams() == false)
    authenticatedNewClient(client);
  else  // チャンネルとか認証以外はここ
    handleCommands(client);
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

void Server::splitCmds(std::string casted_msg, std::vector<std::string> &cmd_with_p) {
  std::string split;
  size_t pos = 0;
  while ((pos = casted_msg.find("\r\n")) != std::string::npos) {
    split = casted_msg.substr(0, pos);
    split[pos] = '\0';
    cmd_with_p.push_back(split);
    casted_msg = casted_msg.substr(pos + 2);
  }
  if (!casted_msg.empty() && (pos = casted_msg.find('\r')) != std::string::npos)
    casted_msg[pos] = '\0';
  if (!casted_msg.empty()) cmd_with_p.push_back(casted_msg);
}

// eraseしたイテレーターを参照しないか確認する！！！member,operator確認
void Server::disconnectClient(ClientData client) {
  int socket = client.getSocket();
  if (clients_.size() == 0) {
    if (close(socket) < 0) putFunctionError("close failed");
  } else {
    std::list<ClientData>::iterator it = clients_.begin();
    std::list<ClientData>::iterator erase_it = it;
    while (it != clients_.end()) {
      if (it->getSocket() == socket) break;
      it++;
    }
    if (close(socket) < 0) putFunctionError("close failed");
    it->setSocket(-1);
  }
  std::cout << "disconnected sockfd : " << socket << std::endl;
}

std::list<ClientData>::iterator Server::eraseClient(std::list<ClientData>::iterator erase_it) {
  ClientData *erase_c = getClientByNickname(erase_it->getNickname());
  if (erase_c != NULL) {
    for (std::map<std::string, Channel *>::iterator it = channels_.begin(); it != channels_.end();
         it++) {
      if (it->second->isMember(erase_c) == true) it->second->removeMember(erase_c);
      if (it->second->isOperator(erase_c) == true) it->second->removeOperator(erase_c);
      if (it->second->CountMember() == 0) {
        std::map<std::string, Channel *>::iterator erase_it = it;
        delete it->second;
        channels_.erase(erase_it);
      }
    }
  }
  return Server::clients_.erase(erase_it);  // 最後にクライアントを削除
}

void Server::putFunctionError(const char *errmsg) {
  perror(errmsg);
  closeAllSocket();
  throw std::exception();
}

ClientData *Server::getClientByNickname(const std::string &nickname) {
  for (std::list<ClientData>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
    if (it->getNickname() == nickname) return &(*it);
  }
  return NULL;
}
