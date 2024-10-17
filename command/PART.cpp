#include "Channel.hpp"
#include "Server.hpp"

void Server::handlePart(const std::string& params, ClientData& client) {
    std::istringstream iss(params);
    std::string channelName;
    std::string partMessage;
    iss >> channelName;
    std::getline(iss, partMessage);
    if (!partMessage.empty() && partMessage[0] == ':') {
        partMessage = partMessage.substr(1);
    }

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
                          "127.0.0.1" + " PART " + channelName + " :" + partMessage + "\r\n";
    channel->broadcastMessage(partMsg, &client);
    channel->removeMember(&client);

  // チャンネルが空になった際
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
