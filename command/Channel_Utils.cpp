#include "../Server.hpp"

Channel* Server::getChannel(const std::string& channelName)
{
    std::map<std::string, Channel>::iterator it = channels_.find(channelName);
    if (it != channels_.end())
        return &(it->second);
    return NULL;
}

Channel* Server::createChannel(const std::string& channelName) 
{
    channels_[channelName] = Channel(channelName);
    return &channels_[channelName];
}