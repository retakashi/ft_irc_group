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

  if (!channel->isMember(&client)) {
    sendCmdResponce(ERR_NOTONCHANNEL, channelName, client);
    return;
  }

  std::string partMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" +
                        getHostname() + " PART :" + channelName;
  channel->broadcastMessage(partMsg, &client);
  channel->removeMember(&client);
  channel->broadcastMessage(client.getNickname() + " has left the channel.", &client);
}
