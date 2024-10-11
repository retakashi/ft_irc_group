#include "../Channel.hpp"
#include "../ClientData.hpp"
#include "../CmdResponse.hpp"
#include "../Server.hpp"

// KICK　<channel> *( "," <channel> ) <user> *( "," <user> )

void Channel::kickMember(ClientData* client, ClientData* target, const std::string& reason) {
  removeMember(target);
  broadcastMessage(client->getNickname() + " has left the channel.",
                   client);  // removeMemberから移動
  std::string message = ":" + client->getNickname() + " KICK " + ch_name_ + " " +
                        target->getNickname() + " :" + reason + "\r\n";
  Server::ft_send(message, *target);
  broadcastMessage(message, client);
}

void Server::handleKick(const std::string& params, ClientData& client) {
  std::istringstream iss(params);
  std::string channelName, targetNickname, reason;
  iss >> channelName >> targetNickname;
  if (iss.str().find(':'))
    reason = iss.str().substr(1);
  else
    reason = "No reason provided";  // Remove leading space

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

  // チャンネルメンバーの確認
  if (!channel->isMember(&client)) {
    sendCmdResponce(ERR_NOTONCHANNEL, channelName, client);
    return;
  }

  // 招待の権限確認
  if (channel->getInviteOnly() && !channel->isOperator(&client)) {
    sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelName, client);
    return;
  }

  // ターゲットユーザーのチャンネル参加確認
  if (!channel->isMember(target)) {
    sendCmdResponce(ERR_USERONCHANNEL, targetNickname, client);
    return;
  }
  channel->kickMember(&client, target, reason);
}
