#include "../Server.hpp"

void Server::handleUSER(std::string param, ClientData& client) {
  struct user_data user_data;
  user_data.mode = '\0';
  if (isValidUSERparams(param, user_data, client) == true) {
    client.setUsername(user_data.username);
    client.setMode(user_data.mode);
    client.setRealname(user_data.realname);
  }
}

// sendCmdResponceの戻り値を0(false)にしている。
bool Server::isValidUSERparams(std::string& params, struct user_data& user_data,
                               const ClientData& client) {
  size_t i = 0;
  std::string::size_type pos = 0;
  if (params.size() == 0) return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
  while (user_data.realname.empty()) {
    pos = params.find(' ');
    if ((user_data.username.empty() || user_data.mode == '\0' || user_data.unused.empty()) &&
        pos == std::string::npos)
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    if (user_data.username.empty()) {
      if (isValidUsername(params, user_data.username, pos) == false)
        return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    } else if (user_data.mode == '\0' || user_data.unused.empty()) {
      if (isValidMiddle(params, user_data.mode, user_data.unused, pos) == false)
        return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    } else if (user_data.realname.empty()) {
      if (isValidRealname(params, user_data.realname) == false)
        return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    }
    if (pos != std::string::npos) params = params.substr(pos + 1);
  }
  return true;
}

// usernameの格納も行う。
bool Server::isValidUsername(const std::string& param, std::string& username,
                             std::string::size_type pos) {
  std::string nospcrlfcl("\0\r\n @", 5);
  username = param.substr(0, pos);
  username[pos] = '\0';
  if (nospcrlfcl.find(username[0]) != std::string::npos || username[0] == ':') return false;
  for (size_t i = 0; i < username.size(); i++) {
    if (nospcrlfcl.find(username[i]) != std::string::npos) return false;
  }
  return true;
}

// mode or unusedの格納も行う。
bool Server::isValidMiddle(const std::string& param, char& mode, std::string& unused,
                           std::string::size_type pos) {
  std::string nospcrlfcl("\0\r\n ", 4);
  std::string middle;
  middle = param.substr(0, pos);
  middle[pos] = '\0';
  if (middle.size() == 0) return false;
  for (size_t i = 0; i < middle.size(); i++) {
    if (nospcrlfcl.find(middle[i]) != std::string::npos) return false;
  }
  if (mode == '\0') {
    if (middle[0] == 'W' || middle[0] == 'I')
      mode = middle[0];
    else
      mode = '*';
  } else
    unused = middle;
  return true;
}

// realnameの格納も行う。
bool Server::isValidRealname(const std::string& param, std::string& realname) {
  bool has_trailing = false;
  std::string nospcrlfcl("\0\r\n", 3);
  if (param.size() == 0) return false;
  if (param[0] == ':') has_trailing = true;
  if (has_trailing == true) {
    if (param.size() == 1)
      return false;
    for (size_t i = 1; i < param.size(); i++) {
      if (nospcrlfcl.find(param[i]) != std::string::npos) return false;
    }
  } else {
    for (size_t i = 0; i < param.size(); i++) {
      if (nospcrlfcl.find(param[i]) != std::string::npos || param[i] == ':' || isspace(param[i]))
        return false;
    }
  }
  realname = param;
  return true;
}
