#include "Server.hpp"

/* 
nicknameは最大20字まで
Command: NICK
Parameters: <nickname> [ <hopcount> ]
*/
bool Server::isValidNickname(std::string& param) {
  std::string::size_type pos = 0;
  pos = param.find(" ");
  if (pos != std::string::npos)
  {
    param = param.substr(0, pos);
    param[pos] = '\0';
  }
  if (param.size() > 20 || param.size() < 1) return printCmdResponce(432, param);
  for (size_t i = 0;i < clients_.size();i++)
  {
    if (clients_[i].getNickname() == param)
      return printCmdResponce(433, param);
  }
  if (!std::isalpha(param[0])) return printCmdResponce(432, param);
  size_t i = 0;
  std::string special = "-[]\\`^{}";
  while (param[i] != '\0') {
    if (!std::isalnum(param[i]) && special.find(param[i]) == std::string::npos) return printCmdResponce(432, param);
    i++;
  }
  return true;
}

