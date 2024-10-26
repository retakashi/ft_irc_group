#include "Channel.hpp"

// INVITE <nickname> <channel>
void Server::handleInvite(const std::string& params, ClientData& client) {
  // パラメータの解析
  std::istringstream iss(params);
  std::string targetNickname, channel_name;
  iss >> targetNickname >> channel_name;
  if (targetNickname.empty() || channel_name.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "INVITE", client);
    return;
  }
  // ターゲットユーザーの存在確認
  ClientData* target = getClientByNickname(targetNickname);
  if (!target) {
    sendCmdResponce(ERR_NOSUCHNICK, targetNickname, client);
    return;
  }
  // チャンネルの存在確認
  Channel* channel = getChannelByName(channel_name);
  if (!channel) {
    sendCmdResponce(ERR_NOSUCHCHANNEL, channel_name, client);
    return;
  }

  // チャンネルメンバーの確認
  if (!channel->isMember(&client) && !channel->isOperator(&client)) {
    sendCmdResponce(ERR_NOTONCHANNEL, channel_name, client);
    return;
  }

  // 招待の権限確認
  if (channel->getInviteOnly() && !channel->isOperator(&client)) {
    sendCmdResponce(ERR_CHANOPRIVSNEEDED, channel_name, client);
    return;
  }

  // ターゲットユーザーのチャンネル参加確認
  if (channel->isMember(target) || channel->isOperator(target)) {
    sendCmdResponce(ERR_USERONCHANNEL, targetNickname, client);
    return;
  }
  channel->inviteMember(&client, target);
}

void Channel::inviteMember(ClientData* client, ClientData* target) {
  invitees_.push_back(target);
  std::string message =
      ":" + client->getNickname() + " INVITE " + target->getNickname() + " :" + ch_name_;
  sendAll(message);
  Server::ft_send(message, *target);
}
