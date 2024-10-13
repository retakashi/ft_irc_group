#include "../Server.hpp"

void Server::handleCommands(ClientData& client) {
  std::vector<std::string> cmd_with_p;
  std::string cmd;
  std::string param;
  
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size <= 0) return;
  std::string casted_msg(msg_, recv_size);
  std::cout << "recv: " << casted_msg << std::endl;
  if (casted_msg.find("\r\n") != std::string::npos)
    splitCmds(casted_msg, cmd_with_p);
  else
    cmd_with_p.push_back(casted_msg);
  for (size_t i = 0; i < cmd_with_p.size(); i++) {
    splitCmdAndParam(casted_msg, cmd, param);
    if (client.getAuth() == true && (cmd == "PASS" || cmd == "USER"))
      sendCmdResponce(ERR_ALREADYREGISTRED, client);
    else if (cmd == "NICK")
      handleNICK(param, client);
    else if (cmd == "PRIVMSG")
      handlePrivateMessage(param, client);
    else if (cmd == "INVITE")
      handleInvite(param, client);
    else if (cmd == "MODE")
      handleMODE(param, client);
    else if (cmd == "JOIN")
      handleJoin(param, client);
    else if (cmd == "TOPIC")
      handleTOPIC(param, client);
    else if (cmd == "KICK")
      handleKick(param, client);
    else if (cmd == "PART")
      handlePart(param, client);
    else  // 無効なコマンドが来た時
      sendCmdResponce(ERR_UNKNOWNCOMMAND, cmd, client);
  }
}
