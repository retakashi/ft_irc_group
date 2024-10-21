#include "Server.hpp"

void Server::handleCommands(ClientData& client) {
  std::vector<std::string> cmd_with_p;
  std::string cmd;
  std::string param;

  std::string msg = ft_recv(client.getSocket());
  if (msg.empty()) return;
  std::cout << "recv: " << msg << std::endl;
  if (msg.find("\r\n") != std::string::npos)
    splitCmds(msg, cmd_with_p);
  else
    cmd_with_p.push_back(msg);
  for (size_t i = 0; i < cmd_with_p.size(); i++) {
    splitCmdAndParam(msg, cmd, param);
    if (client.getAuth() == true && (cmd == "PASS" || cmd == "USER"))
      sendCmdResponce(ERR_ALREADYREGISTRED, client);
    else if (cmd == "NICK")
      handleNICK(param, client);
    else if (cmd == "PRIVMSG")
      handlePrivateMessage(param, client);
    else if (cmd == "INVITE")
      handleInvite(param, client);
    else if (cmd == "MODE")
      handleMode(param, client);
    else if (cmd == "JOIN")
      handleJoin(param, client);
    else if (cmd == "TOPIC")
      handleTopic(param, client);
    else if (cmd == "KICK")
      handleKick(param, client);
    else if (cmd == "PART")
      handlePart(param, client);
    else if (cmd == "LIST")
      handleList(param, client);
    else
      sendCmdResponce(ERR_UNKNOWNCOMMAND, cmd, client);
  }
}
