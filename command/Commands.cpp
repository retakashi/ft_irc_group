#include "../Server.hpp"
#include "../CmdResponse.hpp"
#include "../ClientData.hpp"
#include "../Channel.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>


void Server::handleCommands(ClientData& client) {
  std::string command;
  std::string params;
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size <= 0) return;
  std::string casted_msg(msg_, recv_size);
  splitCmdAndParam(casted_msg, command, params);

  if (command == "NICK")
    handleNICK(params, client);
  else if (command == "PRIVMSG")
    handlePrivateMessage(params, client);
  // else if (command == "OPER")
  //     handleOper(params, client);
  // else if (command == "MODE")
  //   handleMODE(params, client);
  // else if (command == "NOTICE")
  //     handleNotice(params, client);
  else if (command == "JOIN")
      handleJoin(params, client);
  // else if (command == "TOPIC")
  //     setTopic(params, client);
  // else if (command == "KICK")
  //     kickClient(params, client);
  // else if (command == "INVITE")
  //     handleInvite(params, client);
  // else if (command == "PART")
  //     handlePart(params, client);
  // else if (command == "QUIT")
  //     handleQuit(params, client);
  else  // 無効なコマンドが来た時
    sendCmdResponce(ERR_UNKNOWNCOMMAND, command, client);
}
