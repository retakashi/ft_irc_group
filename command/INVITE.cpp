#include "../Channel.hpp"

void Channel::inviteMember(ClientData* client, ClientData* target) {
  this->addMember(target);
  std::string message =
      ":" + client->getNickname() + " INVITE " + target->getNickname() + " :" + ch_name_ + "\r\n";
  Server::ft_send(message, *target);
  broadcastMessage(message, client);
  // Server::sendCmdResponce(RPL_INVITING, targetNickname + " " + channelName, client);
}

void Server::handleInvite(const std::string& params, ClientData& client) {
  // パラメータの解析
  std::istringstream iss(params);
  std::string targetNickname, channelName;
  iss >> targetNickname >> channelName;
  if (targetNickname.empty() || channelName.empty()) {
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
  if (channel->isMember(target)) {
    sendCmdResponce(ERR_USERONCHANNEL, targetNickname, client);
    return;
  }

  // 招待の送信
  // std::string message = ":" + client.getNickname() + " INVITE " + target->getNickname() + " :" +
  // channelName + "\r\n"; Server::ft_send(message, *target);
  channel->inviteMember(&client, target);
  sendCmdResponce(RPL_INVITING, targetNickname + " " + channelName, client);
}
