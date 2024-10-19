#include "Channel.hpp"
#include "Server.hpp"

void Server::handleList(const std::string& params, ClientData& client) {
    std::istringstream iss(params);
    std::string channelName;
    iss >> channelName;

    std::string listStart = ":" + servername_ + " 321 " + client.getNickname() + " Channel :Users  Name\r\n";
    ft_send(listStart, client);

    if (channelName.empty()) {
        for (std::map<std::string, Channel*>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
            Channel* channel = it->second;
            std::string channelInfo = ":" + servername_ + " 322 " + client.getNickname() + " " + channel->getName() + " " + std::to_string(channel->getMemberCount()) + " :" + channel->getTopic() + "\r\n";
            ft_send(channelInfo, client);
        }
    } else {
        Channel* channel = getChannelByName(channelName);
        if (channel) {
            std::string channelInfo = ":" + servername_ + " 322 " + client.getNickname() + " " + channel->getName() + " " + std::to_string(channel->getMemberCount()) + " :" + channel->getTopic() + "\r\n";
            ft_send(channelInfo, client);
        } else {
            std::string errorMsg = createCmdRespMsg(servername_, ERR_NOSUCHCHANNEL, client.getNickname(), channelName + " :No such channel");
            ft_send(errorMsg, client);
        }
    }

    std::string listEnd = ":" + servername_ + " 323 " + client.getNickname() + " :End of /LIST\r\n";
    ft_send(listEnd, client);
}

Channel* Server::getChannelByName(const std::string& channelName) {
    std::map<std::string, Channel*>::iterator it = channels_.find(channelName);
    if (it != channels_.end()) {
        return it->second;
    }
    return nullptr;
}

void Server::deleteChannel(const std::string& channelName) {
    std::map<std::string, Channel*>::iterator it = channels_.find(channelName);
    if (it != channels_.end()) {
        delete it->second;
        channels_.erase(it);
    }
}