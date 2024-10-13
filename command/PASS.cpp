#include "../Server.hpp"

void Server::handlePass(std::string param, ClientData& client)
{
  if (client.getAuth())
  {
    Server::sendCmdResponce(ERR_ALREADYREGISTRED,client);
    return ;
  }
  if (param.empty())
  {
    Server::sendCmdResponce(ERR_NEEDMOREPARAMS,"PASS",client);
    return ;
  }
  std::cout << "serverpass_: " << serverpass_ << ",   param: " << param << "!" << std::endl;
  if (this->serverpass_ != param)
  {
    Server::sendCmdResponce(ERR_PASSWDMISMATCH, client);
    return ;
  }
  client.setAuth(true);
}
