#include "Server.hpp"

void Server::handlePrivateMessage(const std::string param, ClientData &client) {
  std::string targets, message;
  size_t spacePos = param.find(' ');

  if (spacePos != std::string::npos) {
    targets = param.substr(0, spacePos);

    size_t colonPos = param.find(':', spacePos);
    if (colonPos != std::string::npos)
      message = param.substr(colonPos + 1);
    else {
      sendCmdResponce(ERR_NOTEXTTOSEND, client);
      return;
    }
  } else {
    sendCmdResponce(ERR_NORECIPIENT, "PRIVMSG",client);
    return;
  }
  if (targets[0] == '#')
    handle_privmsg_channel(targets, message, client);
  else
    handle_privmsg_personal(targets, message, client);
}

void Server::handle_privmsg_channel(std::string targets, std::string message, ClientData &client) {
  std::istringstream iss(targets);
  std::string target;
  std::string errorMsg = ":";

  while (getline(iss, target, '.')) {
    Channel *channel = getChannelByName(target);
    if (!channel)
      errorMsg = createCmdRespMsg(servername_, client.getNickname(), ERR_NOSUCHCHANNEL, target);
    else if (!(channel->isMember(&client)) && !(channel->isOperator(&client)))
      errorMsg = createCmdRespMsg(servername_, client.getNickname(), ERR_NOTONCHANNEL, channel->getChannelname());
    if (errorMsg != ":")
      ft_send(errorMsg, client);
    else {
      // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
      std::string message_ch = ":" + client.getNickname() + " PRIVMSG " + target + " :" + message;
      channel->broadcastMessage(message_ch, &client);
    }
    errorMsg = ":";
  }
}

void Server::handle_privmsg_personal(std::string targets, std::string message, ClientData &client) {
  std::istringstream iss(targets);
  std::string target;
  std::string errorMsg = ":";

  while (getline(iss, target, '.')) {
    ClientData *recipient = getClientByNickname(target);
    if (!recipient)
      errorMsg = createCmdRespMsg(servername_, client.getNickname(), ERR_NOSUCHNICK, target);
    else if (recipient == &client)
      errorMsg = createCmdRespMsg(servername_, client.getNickname(), ERR_NORECIPIENT, "PRIVMSG");
    if (errorMsg != ":")
      ft_send(errorMsg, client);
    else {
      // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
      std::string recipientMessage = ":PRIVMSG " + client.getNickname() + " :" + message;
      ft_send(recipientMessage, *recipient);
    }
    errorMsg = ":";
  }
}
