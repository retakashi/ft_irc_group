#include "Channel.hpp"
#include "Server.hpp"
#include "CmdResponse.hpp"

// LIST <channel> *( "," <channel> ) [ <target> ]

void Server::handleList(const std::string& params, ClientData& client) {
    std::istringstream iss(params);
    std::string channelName;
    iss >> channelName;

    // std::string listStart = ":" + servername_ + " 321 " + client.getNickname() + " Channel :Users";
    // ft_send(listStart, client);

    if (channelName.empty()) {
        for (std::map<std::string, Channel*>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
            Channel* channel = it->second;
            std::string channelInfo =  ":server 322 " + client.getNickname() + " " + channel->getChannelname() + " " + std::to_string(channel->CountMembers()) + " :" + channel->getTopic();
            // std::string channelInfo = ":" + servername_ + " 322 " + client.getNickname() + " " + channel->getChannelname() + " " + std::to_string(channel->getMembers().size()+ 1) + " :" + channel->getTopic() + "\r\n";
            ft_send(channelInfo, client);
        }
    } else {
        Channel* channel = getChannelByName(channelName);
        if (channel) {
            std::string channelInfo =  ":server 322 " + client.getNickname() + " " + channel->getChannelname() + " " + std::to_string(channel->CountMembers()) + " :" + channel->getTopic();
            // std::string channelInfo = ":" + servername_ + " 322 " + client.getNickname() + " " + channel->getChannelname() + " " + std::to_string(channel->getMembers().size() + 1) + " :" + channel->getTopic() + "\r\n";
            ft_send(channelInfo, client);
        } else {
            std::string errorMsg = createCmdRespMsg(servername_, client.getNickname(), ERR_NOSUCHCHANNEL,  channelName + " :No such channel");
            ft_send(errorMsg, client);
        }
    }

    std::string listEnd = ":" + servername_ + " 323 " + client.getNickname() + " :End of /LIST";
    ft_send(listEnd, client);
}
