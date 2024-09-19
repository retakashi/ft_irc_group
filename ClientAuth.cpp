#include "Server.hpp"

/* PASSは一番最初に認証しなければならない(MUST) */
void Server::authenticatedNewClient(ClientData& client) {
  std::string command;
  std::string param;
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size == 0) return;
  std::string casted_msg(msg_, recv_size);
  splitCmdAndParam(casted_msg, command, param);
  if (!client.getAuth() && command != "PASS") {
    sendCmdResponce(ERR_NOTREGISTERED, client);
    return;
  }
  if (command == "PASS") handlePass(param, client);
  else if (client.getAuth()) {
    if (command != "NICK" && command != "USER") {
      sendCmdResponce(ERR_NOTREGISTERED, client);
      return;
    }
    if (command == "NICK")
      handleNICK(param, client);
    else if (command == "USER")
      handleUSER(param, client);
  }
  if (client.isCompleteAuthParams() == true) sendWelcomeToIrc(client);
}

void Server::handlePass(std::string param, ClientData& client)
{
  std::cout << "starting PASS authentication: " << param << std::endl;
  if (client.getAuth())
  {
    sendCmdResponce(ERR_ALREADYREGISTRED,client);
    return ;
  }
  if (param.empty())
  {
    sendCmdResponce(ERR_NEEDMOREPARAMS,"PASS",client);
    return ;
  }
  if (this->serverpass_ != param)
  {
    sendCmdResponce(ERR_PASSWDMISMATCH, client);
    return ;
  }
  client.setAuth(true);
}

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
  if (params.size() == 0) return sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
  while (user_data.realname.empty()) {
    pos = params.find(' ');
    if ((user_data.username.empty() || user_data.mode == '\0' || user_data.unused.empty()) &&
        pos == std::string::npos)
      return sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    if (user_data.username.empty()) {
      if (isValidUsername(params, user_data.username, pos) == false)
        return sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    } else if (user_data.mode == '\0' || user_data.unused.empty()) {
      if (isValidMiddle(params, user_data.mode, user_data.unused, pos) == false)
        return sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
    } else if (user_data.realname.empty()) {
      if (isValidRealname(params, user_data.realname) == false)
        return sendCmdResponce(ERR_NEEDMOREPARAMS, "USER", client);
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

// mode or unusedの格納も行う。
bool Server::isValidMiddle(const std::string& params, char& mode, std::string& unused,
                           std::string::size_type pos) {
  std::string nospcrlfcl("\0\r\n ", 5);
  std::string middle;
  middle = params.substr(0, pos);
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
bool Server::isValidRealname(const std::string& params, std::string& realname) {
  std::string nocrlfcl("\0\r\n", 4);
  std::string nospcrlfcl("\0\r\n ", 5);
  if (params.size() == 0) return false;
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
  ss << ":" << servername_ << " 001 Welcome to the Internet Relay Network " << client.getNickname()
     << "!" << client.getUsername() << "@" << hostname_;
  send_size = createSendMsg(ss.str());
  ft_send(client, send_size);
}
