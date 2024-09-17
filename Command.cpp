#include "Server.hpp"

void Server::handleNICK(std::string param, ClientData& client) {
  if (isValidNickname(param, client) == true) client.setNickname(param);
}

/*
nicknameは最大20字まで。第一引数以外は無視
Command: NICK
Parameters: <nickname> [ <hopcount> ]
sendCmdResponceの戻り値を0(false)にしている。
*/
bool Server::isValidNickname(std::string& param, const ClientData& client) {
  // paramを第一引数のみにする。
  std::string::size_type pos = 0;
  pos = param.find(" ");
  if (pos != std::string::npos) {
    param = param.substr(0, pos);
    param[pos] = '\0';
  }
  if (client.getNickname().empty() && param.size() == 0)
    return sendCmdResponce(ERR_NONICKNAMEGIVEN, client);
  if (param.size() == 0) {
    std::stringstream ss;
    ss << "Your nickname is " << client.getNickname();
    size_t size = createSendMsg(ss.str());
    ft_send(client.getSocket(), size);
    return false;
  }
  if (param.size() > 20) return sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
  for (size_t i = 0; i < clients_.size(); i++) {
    if (clients_[i].getNickname() == param)
      return sendCmdResponce(ERR_NICKNAMEINUSE, param, client);
  }
  std::string special = "[]\\`_^{}|";
  if (!std::isalpha(param[0]) && special.find(param[0]) == std::string::npos)
    return sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
  for (size_t i = 1; param[i] != '\0'; i++) {
    if (!std::isalnum(param[i]) && special.find(param[i]) == std::string::npos && param[i] != '-')
      return sendCmdResponce(ERR_ERRONEUSNICKNAME, param, client);
  }
  return true;
}


void Server::handle_pass(std::string param, ClientData& client)
{
  std::cout << "starting PASS authentication: " << param << std::endl;
  if (client.getAuth())
  {
    std::cout << "Already authenticated client :" << client.getSocket() << std::endl;
    return ;
  }
  if (this->serverpass_ == param)
    client.setAuth(true);
  else 
    sendCmdResponce(ERR_PASSWDMISMATCH, param, client);
}




void Server::handle_commands(std::string command, std::string params, ClientData& client) 
{

    // ひとまず思いつく限りのコマンドにしましたが、適宜追加して行く。NICK ,USER ,PASSは既存のものに投げています。まだ、使用はしていない。
    if (command == "PASS") 
        handle_pass(params, client); 
    else if (command == "JOIN") 
        handle_join(params, client); 
    else if (command == "NICK") 
        handleNICK(params, client); 
    else if (command == "USER") 
        handleUSER(params, client); 
    else if (command == "KICK") 
        handle_kick(params, client); 
    else 
    {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void Server::handle_join(const std::string& params, ClientData& client) 
{
    // チャンネルについてここに実装
    std::cout << "JOIN command processed" << std::endl;
}

void Server::handle_kick(const std::string& params, ClientData& client) 
{
    // チャンネルについてここに実装
    std::cout << "JOIN command processed" << std::endl;
}