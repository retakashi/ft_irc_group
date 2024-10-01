#include "../Server.hpp"

void Server::handlePass(std::string param, ClientData& client)
{
  std::cout << "starting PASS authentication: " << param << std::endl;
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
  if (this->serverpass_ != param)
  {
    Server::sendCmdResponce(ERR_PASSWDMISMATCH, client);
    return ;
  }
  client.setAuth(true);
}
