#include "Server.hpp"

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
  std::string special = "-[]\\`^{}";
  for (size_t i = 0; param[i] != '\0';i++) {
    if (!std::isalpha(param[0]) ||
        (!std::isalnum(param[i]) && special.find(param[i]) == std::string::npos)) {
      sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
      return false;
    }
  }
  return true;
}

/* realname　':'の後に任意のオクテット列（バイト列）で、SPACE、NUL、CR、CF
を含むことはできない。->エラーにする */
bool Server::isUserParamCountValid(const std::string& params, const ClientData& client) {
  size_t space_cnt = 0;
  size_t i = 0;
  while (params[i] != '\0') {
    if (params[i] == ' ') {
      space_cnt++;
      while (params[i] == ' ') i++;
      if (params[i] == '\0') break;
    }
    if (params[i] == ':' && (params[i + 1] == '\0' || params[i + 1] == '\r' ||
                             params[i + 1] == '\n' || params[i + 1] == ' '))
    {
      sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
      return false;
    }
    i++;
  }
  if (space_cnt < 3)
  {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client); 
    return false;
  }
  return true;
}

bool Server::isCompleteAuthParams(const ClientData& client) {
  if (client.getNickname().empty() || client.getUsername().empty() || client.getHostname().empty() || client.getServername().empty() || client.getRealname().empty())
    return false;
  return true;
}

bool Server::isCompleteUserParams(const ClientData& client) {
  if (client.getUsername().empty() || client.getHostname().empty() || client.getServername().empty() || client.getRealname().empty())
    return false;
  sendCmdResponce(ERR_ALREADYREGISTRED, client);
  return true; 
}

void Server::sendWelcomeToIrc(const ClientData &client) {
  std::stringstream ss;
  size_t send_size = 0;
  ss << ":001 Welcome to the Internet Relay Network " << client.getNickname() << "!"
     << client.getUsername() << "@" << client.getHostname();
  send_size = createSendMsg(ss.str());
  ft_send(client, send_size);
}
