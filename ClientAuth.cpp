#include "Server.hpp"

/* PASSをループ前に追加
PASSは一番最初に認証しなければならない(MUST) */
void Server::authenticatedNewClient(int client_sock) {
  std::string command;
  std::string::size_type pos = 0;
  ClientData new_client(client_sock);

  while (isCompleteAuthParams(new_client) == false) {
    size_t recv_size;
    recv_size = ft_recv(client_sock);
    std::string casted_msg(msg_, recv_size);
    pos = splitCommand(casted_msg, command);
    if (command != "NICK" && command != "USER") {
      sendCmdResponce(ERR_NOTREGISTERED, new_client);
      continue;
    }
    if (command == "NICK")
      NICKcmd(casted_msg, pos, new_client);
    else if (command == "USER")
      USERcmd(casted_msg, pos, new_client);
  }
  clients_.push_back(new_client);
  sendWelcomeToIrc(new_client);
}

bool Server::isCompleteAuthParams(const ClientData& client) {
  if (client.getNickname().empty() || client.getRealname().empty())
    return false;
  return true;
}

void Server::USERcmd(std::string casted_msg, std::string::size_type pos, ClientData &client) {
  std::string param;
  struct user_data user_data;
  user_data.mode = '\0';
  splitParam(casted_msg, param, pos);
  if (isValidUSERcmdParams(param, user_data, client) == true) {
    client.setUsername(user_data.username);
    client.setMode(user_data.mode);
    client.setRealname(user_data.realname);
  }
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

bool Server::isValidUSERcmdParams(std::string& params, struct user_data& user_data,
                               const ClientData& client) {
  size_t i = 0;
  std::string::size_type pos = 0;
  if (params.size() == 0) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    return false;
  }
  while (user_data.realname.empty()) {
    pos = params.find(' ');
    if ((user_data.username.empty() || user_data.mode == '\0' || user_data.unused.empty()) &&
        pos == std::string::npos) {
      sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
      return false;
    }
    if (user_data.username.empty()) {
      if (isValidUsername(params, user_data.username, pos) == false) {
        sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
        return false;
      }
    } else if (user_data.mode == '\0' || user_data.unused.empty()) {
      if (isValidMiddle(params, user_data.mode, user_data.unused, pos) == false) {
        sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
        return false;
      }
    } else if (user_data.realname.empty()) {
      if (isValidRealname(params, user_data.realname) == false) {
        sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
        return false;
      }
    }
    if (pos != std::string::npos) params = params.substr(pos + 1);
  }
  return true;
}

// usernameの格納も行う。
bool Server::isValidUsername(const std::string& params, std::string& username,
                             std::string::size_type pos) {
  std::string nospcrlfcl("\0\r\n @", 6);
  username = params.substr(0, pos);
  username[pos] = '\0';
  if (nospcrlfcl.find(username[0]) != std::string::npos || username[0] == ':') return false;
  for (size_t i = 0; i < username.size(); i++) {
    if (nospcrlfcl.find(username[i]) != std::string::npos) return false;
  }
  return true;
}

//mode or unusedの格納も行う。
bool Server::isValidMiddle(const std::string& params, char& mode, std::string& unused,
                           std::string::size_type pos) {
  std::string nospcrlfcl("\0\r\n ", 5);
  std::string middle;
  middle = params.substr(0, pos);
  middle[pos] = '\0';
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

//realnameの格納も行う。
bool Server::isValidRealname(const std::string& params, std::string& realname) {
  std::string nocrlfcl("\0\r\n", 4);
  std::string nospcrlfcl("\0\r\n ", 5);
  bool is_trailing = false;
  if (params[0] == ':') is_trailing = true;
  for (size_t i = 0; i < params.size(); i++) {
    if (is_trailing == true && nocrlfcl.find(params[i]) != std::string::npos)
      return false;
    else if (is_trailing == false && nospcrlfcl.find(params[i]) != std::string::npos)
      return false;
  }
  realname = params;
  return true;
}

void Server::sendWelcomeToIrc(const ClientData& client) {
  std::stringstream ss;
  size_t send_size = 0;
  ss << ":"<< servername_ << " 001 Welcome to the Internet Relay Network " << client.getNickname() << "!"
     << client.getUsername() << "@" << hostname_;
  send_size = createSendMsg(ss.str());
  ft_send(client, send_size);
}
