#include "../Channel.hpp"

void Channel::inviteMember(ClientData* client, ClientData* target) 
{ 
    this->addClient(target);
    std::string message = ":" + client->getNickname() + " INVITE " + target->getNickname() + " :" + ch_name_ + "\r\n";
    Server::ft_send(message, *target);
    broadcastMessage(message, client);
    // Server::sendCmdResponce(RPL_INVITING, targetNickname + " " + channelName, client);
}
