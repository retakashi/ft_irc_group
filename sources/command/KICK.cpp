#include "Channel.hpp"
#include "Server.hpp"

// KICK　<channel> *( "," <channel> ) <user> *( "," <user> )
void Server::handleKick(const std::string& params, ClientData& client) {
  std::istringstream iss(params);
  std::string channel_name, target_nickname, reason;
  reason = "";
  iss >> channel_name >> target_nickname >> reason;
  if (reason == ":") reason = "No reason provided";
  // ターゲットユーザーの存在確認
  ClientData* target = getClientByNickname(target_nickname);
  if (!target) {
    sendCmdResponce(ERR_NOSUCHNICK, target_nickname, client);
    return;
  }
  // チャンネルの存在確認
  Channel* channel = getChannelByName(channel_name);
  if (!channel) {
    sendCmdResponce(ERR_NOSUCHCHANNEL, channel_name, client);
    return;
  }
  // オペレーター確認
  if (!(channel->isOperator(&client))) {
    sendCmdResponce(ERR_CHANOPRIVSNEEDED, channel_name, client);
    return;
  }
  // ターゲットユーザーのチャンネル参加確認
  if (!channel->isMember(target) && !channel->isOperator(target)) {
    sendCmdResponce(ERR_USERONCHANNEL, target_nickname, client);
    return;
  }
  channel->kickMember(&client, target, reason);

  // メンバーがいなくなった時の処理
  if (channel->CountMembers() == 0) {
    std::map<std::string, Channel*>::iterator it = channels_.find(channel->getChannelname());
    if (it->second->CountMembers() == 0) {
      std::cout << "Channel " << channel->getChannelname() << " has been deleted." << std::endl;
      std::map<std::string, Channel*>::iterator erase_it = it;
      delete it->second;
      channels_.erase(erase_it);
    }
  }
}

void Channel::kickMember(ClientData* client, ClientData* target, const std::string& reason) {
  std::string partMessage = ":" + target->getNickname() + "!" + target->getUsername() + "@127.0.0.1" + " PART " + this->getChannelname() +
                            " :Kicked by " + client->getNickname();
  Server::ft_send(partMessage, *target);

  if (this->CountMembers() < 1) return;
  // その他のユーザの処理
  std::string message = ":" + client->getNickname() + "!" + client->getUsername() + "@127.0.0.1" + " KICK " + this->getChannelname() + " " +
                        target->getNickname() + " " + reason;
  sendAll(message);
  // 除外されるユーザーの処理
  if (isMember(target))
    removeMember(target);
  else if (isOperator(target))
    removeOperator(target);
}
