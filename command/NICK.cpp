#include "../Server.hpp"

void Server::handleNICK(std::string param, ClientData& client) {
  if (isValidNickname(param, client) == true) {
    std::string old_nick = client.getNickname();
    client.setNickname(param);
    Server::ft_send(":reira NICK "+ client.getNickname(), client);
  }
}

/*
nicknameは最大20字まで。
Command: NICK
Parameters: <nickname>
sendCmdResponceの戻り値を0(false)にしている。
*/
bool Server::isValidNickname(std::string& param, const ClientData& client) {
  if (client.getNickname().empty() && param.size() == 0)
    return Server::sendCmdResponce(ERR_NONICKNAMEGIVEN, client);
  if (param.size() == 0) {
    Server::ft_send("Your nickname is " + client.getNickname(), client);
    return false;
  }
  if (param.size() > 20) return Server::sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
  for (std::list<ClientData>::iterator it = Server::clients_.begin(); it != Server::clients_.end();
       it++) {
    if (&client != &(*it) && it->getNickname() == param)
      return Server::sendCmdResponce(ERR_NICKNAMEINUSE, param, client);
  }
  std::string special = "[]\\`_^{}|-";
  if (!std::isalpha(param[0]))
    return Server::sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
  for (size_t i = 1; param[i] != '\0'; i++) {
    if (!std::isalnum(param[i]) && special.find(param[i]) == std::string::npos)
      return Server::sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
  }
  return true;
}
