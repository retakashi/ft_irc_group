#include "../Server.hpp"

void Server::handleCommands(ClientData& client) {
  std::string command;
  std::string params;
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size == 0) return;
  std::string casted_msg(msg_, recv_size);
  splitCmdAndParam(casted_msg, command, params);
  if (command == "NICK")
    handleNICK(params, client);
  else if (command == "PRIVMSG")
    handlePrivateMessage(params, client);
  // else if (command == "OPER")
  //     handleOper(params, client);
  else if (command == "MODE")
    handleMODE(params, client);
  // else if (command == "NOTICE")
  //     handleNotice(params, client);
  // if (command == "JOIN")
  //     handleJoin(params, client);
  // else if (command == "TOPIC")
  //     handleTopic(params, client);
  // else if (command == "KICK")
  //     handleKick(params, client);
  // else if (command == "PART")
  //     handlePart(params, client);
  // else if (command == "QUIT")
  //     handleQuit(params, client);
  else  // 無効なコマンドが来た時
    sendCmdResponce(ERR_UNKNOWNCOMMAND, command, client);
}
