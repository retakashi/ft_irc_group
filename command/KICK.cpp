#include "../Channel.hpp"
#include "../ClientData.hpp"
#include "../CmdResponse.hpp"
#include "../Server.hpp"

// KICK　<channel> *( "," <channel> ) <user> *( "," <user> )

void Channel::kickMember(ClientData* client, ClientData* target, const std::string& reason) {
  // 除外されるユーザーの処理
  if (isMember(target))
    removeMember(target);
  else if (isOperator(target))
    removeOperator(target);
  std::string partMessage = ":" + target->getNickname() + " PART " + this->getChannelname() +
                            " :Kicked by " + client->getNickname() + "\r\n";
  Server::ft_send(partMessage, *target);

  // その他のユーザの処理
  std::string message = ":" + client->getNickname() + " KICK " + this->getChannelname() + " " +
                        target->getNickname() + " " + reason + "\r\n";
  sendAll(message);
}

void Server::handleKick(const std::string& params, ClientData& client) {
  std::istringstream iss(params);
  std::string channelName, targetNickname, reason;
  reason = "";
  iss >> channelName >> targetNickname >> reason;
  if (reason == ":") reason = "No reason provided";
  // ターゲットユーザーの存在確認
  ClientData* target = getClientByNickname(targetNickname);
  if (!target) {
    sendCmdResponce(ERR_NOSUCHNICK, targetNickname, client);
    return;
  }
  // チャンネルの存在確認
  Channel* channel = getChannelByName(channelName);
  if (!channel) {
    sendCmdResponce(ERR_NOSUCHCHANNEL, channelName, client);
    return;
  }
  // オペレーター確認
  if (!(channel->isOperator(&client))) {
    sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelName, client);
    return;
  }
  // ターゲットユーザーのチャンネル参加確認
  if (!channel->isMember(target) && !channel->isOperator(target)) {
    sendCmdResponce(ERR_USERONCHANNEL, targetNickname, client);
    return;
  }
  channel->kickMember(&client, target, reason);
}
