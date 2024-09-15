#include "Server.hpp"

void Server::NICKcmd(std::string casted_msg, std::string::size_type pos, ClientData &client) {
  std::string param;
  splitParam(casted_msg, param, pos);
  if (isValidNickname(param, client) == true) client.setNickname(param);
}
