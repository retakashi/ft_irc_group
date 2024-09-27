#include "Channel.hpp"
#include "Server.hpp"
#include <algorithm>

Channel::Channel(const std::string& name) : name_(name) {}

Channel::~Channel() {}

void Channel::addClient(ClientData* client) {
    clients_.push_back(client);
    broadcastMessage(client->getNickname() + " has joined the channel.");
}

void Channel::removeClient(ClientData* client) {
    clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
    broadcastMessage(client->getNickname() + " has left the channel.");
}

bool Channel::isOperator(ClientData* client) const {
    return std::find(operators_.begin(), operators_.end(), client) != operators_.end();
}

void Channel::broadcastMessage(const std::string& message, ClientData* sender) {
    for (std::vector<ClientData*>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        if (*it != sender) {
             ft_send(**it, createSendMsg(message));
        }
    }
}

void Channel::kickClient(ClientData* client, ClientData* target, const std::string& reason) {
    if (isOperator(client)) {
        removeClient(target);
        std::string kickMsg = createCmdRespMsg(client->getServerName(), "KICK " + name_ + " " + target->getNickname() + " :" + reason);
        ft_send(*target, createSendMsg(kickMsg));
    }
}

void Channel::inviteClient(ClientData* client, ClientData* target) {
    if (isOperator(client)) {
        std::string inviteMsg = createCmdRespMsg(client->getServerName(), "INVITE " + target->getNickname() + " :" + name_);
        ft_send(*target, createSendMsg(inviteMsg));
        // Add logic to actually add the client to the channel if needed
    }
}

void Channel::setTopic(ClientData* client, const std::string& topic) {
    if (isOperator(client)) {
        topic_ = topic;
        std::string topicMsg = createCmdRespMsg(client->getServerName(), "TOPIC " + name_ + " :" + topic);
        broadcastMessage(topicMsg, client);
    }
}

const std::string& Channel::getTopic() const { return topic_; }

const std::vector<ClientData*>& Channel::getClients() const { return clients_; }

void Channel::setMode(ClientData* client, char mode, bool enable) {
    if (isOperator(client)) {
        // Implement mode setting logic here
    }
}