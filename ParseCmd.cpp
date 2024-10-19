#include "Server.hpp"

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
