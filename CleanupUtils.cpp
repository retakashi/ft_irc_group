#include "Server.hpp"

void Server::disconnectClient(ClientData client) {
  int socket = client.getSocket();
  if (Server::clients_.size() == 0) {
    if (close(socket) < 0) putFunctionError("close failed");
  } else {
    std::list<ClientData>::iterator it = Server::clients_.begin();
    while (it != Server::clients_.end()) {
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
    for (std::map<std::string, Channel *>::iterator it = channels_.begin(); it != channels_.end();) {
      if (it->second->isMember(erase_c) == true) it->second->removeMember(erase_c);
      if (it->second->isOperator(erase_c) == true) it->second->removeOperator(erase_c);
      if (it->second->CountMembers() == 0) {
        std::map<std::string, Channel *>::iterator erase_it = it;
        it++;
        delete erase_it->second;
        channels_.erase(erase_it);
      }
      else
        it++;
    }
  }
  return Server::clients_.erase(erase_it);
}

void Server::closeAllSocket() {
  for (std::list<ClientData>::iterator it = Server::clients_.begin(); it != Server::clients_.end();
       it++) {
    if (it->getSocket() != -1 && close(it->getSocket()) < 0) perror("close failed");
  }
  if (serversock_ != 0 && close(serversock_) < 0) perror("close failed");
}
