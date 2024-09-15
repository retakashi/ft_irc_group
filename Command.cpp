#include "Server.hpp"

void Server::handleNICK(std::string casted_msg, std::string::size_type pos, ClientData &client) {
  std::string param;
  splitParam(casted_msg, param, pos);
  if (isValidNickname(param, client) == true) client.setNickname(param);
}

/*
nicknameは最大20字まで。第一引数以外は無視
Command: NICK
Parameters: <nickname> [ <hopcount> ]
*/
bool Server::isValidNickname(std::string& param, const ClientData& client) {
  std::string::size_type pos = 0;
  if (param.size() <= 0) {
    sendCmdResponce(ERR_NONICKNAMEGIVEN, client);
    return false;
  }
  pos = param.find(" ");
  if (pos != std::string::npos) {
    param = param.substr(0, pos);
    param[pos] = '\0';
  }
  if (param.size() > 20 || param.size() < 1) {
    sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
    return false;
  }
  for (size_t i = 0; i < clients_.size(); i++) {
    if (clients_[i].getNickname() == param) {
      sendCmdResponce(ERR_NICKNAMEINUSE, param, client);
      return false;
    }
  }
  std::string special = "[]\\`_^{}|";
  if (!std::isalpha(param[0]) && special.find(param[0]) == std::string::npos) {
    sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
    return false;
  }
  for (size_t i = 1; param[i] != '\0'; i++) {
    if (!std::isalnum(param[i]) && special.find(param[i]) == std::string::npos && param[i] != '-') {
      sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
      return false;
    }
  }
  return true;
}
