#include "Channel.hpp"
#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Server.hpp"

// KICK　<channel> *( "," <channel> ) <user> *( "," <user> )
void Channel::removeClient(ClientData* client) {
    member_.erase(std::remove(member_.begin(), member_.end(), client), member_.end());
    broadcastMessage(client->getNickname() + " has left the channel.");
}

bool Channel::isOperator(ClientData* client) const {
    return std::find(operators_.begin(), operators_.end(), client) != operators_.end();
}

void Channel::kickMember(ClientData* client, ClientData* target, const std::string& reason) { 
    if (isOperator(client)) {
        std::vector<ClientData*>::iterator it = std::find(member_.begin(), member_.end(), target);
        if (it != member_.end()) {
            removeClient(target);
            std::string message = ":" + client->getNickname() + " KICK " + ch_name_ + " " + target->getNickname() + " :" + reason + "\r\n";
            broadcastMessage(message, client);
        } else {
            std::cerr << "Target client not found in the channel." << std::endl;
        }
    } else {
        std::cerr << "Client is not an operator." << std::endl;
    }
}
