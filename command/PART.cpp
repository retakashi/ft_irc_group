#include "Channel.hpp"
#include "Server.hpp"

void Server::handlePart(const std::string& params, ClientData& client) {
  std::istringstream iss(params);
  std::string channelName;
  iss >> channelName;

  if (channelName.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "PART", client);
    return;
  }

  Channel* channel = getChannelByName(channelName);
  if (!channel) {
    sendCmdResponce(ERR_NOSUCHCHANNEL, channelName, client);
    return;
  }

  if (!channel->isMember(&client) && !channel->isOperator(&client)) {
    sendCmdResponce(ERR_NOTONCHANNEL, channelName, client);
    return;
  }
  std::string partMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + getHostname() + " PART " + channelName;
  channel->sendAll(partMsg);
  if (channel->isMember(&client))
    channel->removeMember(&client);
  if (channel->isOperator(&client))
    channel->removeOperator(&client);

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
