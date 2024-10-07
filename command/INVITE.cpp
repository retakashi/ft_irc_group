#include "Channel.hpp"

void Channel::inviteMember(ClientData* client, ClientData* target) { 
    if (isOperator(client)) {
        if (std::find(member_.begin(), member_.end(), target) == member_.end()) {
            std::string message = ":" + client->getNickname() + " INVITE " + target->getNickname() + " :" + ch_name_ + "\r\n";
            Server::ft_send(message, *target);
            broadcastMessage(message, client);
        } else {
            std::cerr << "Target client is already in the channel." << std::endl;
        }
    } else {
        std::cerr << "Client is not an operator." << std::endl;
    }
}
