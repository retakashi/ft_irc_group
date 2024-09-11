#include "Channel.hpp"
#include "ClientData.hpp"
#include <algorithm>

Channel::Channel() {}

Channel::Channel(const std::string& name) : name(name) {}

void Channel::addClient(ClientData* client) {
    clients_.insert(client);
}

void Channel::removeClient(ClientData* client) {
    clients_.erase(client);
}

void Channel::broadcastMessage(const std::string& message, ClientData* sender) {
    for (ClientData* client : clients_) {
        if (client != sender) {
            client->sendMessage(message);
        }
    }
}

void Channel::kickClient(ClientData* client, ClientData* target) {
    if (isOperator(client)) {
        removeClient(target);
        target->sendMessage("You have been kicked from the channel: " + name);
    }
}

void Channel::inviteClient(ClientData* client, ClientData* target) {
    if (isOperator(client)) {
        target->sendMessage("You have been invited to the channel: " + name);
        // Add logic to actually add the client to the channel if needed
    }
}

void Channel::setTopic(ClientData* client, const std::string& topic) {
    if (isOperator(client)) {
        topic = topic;
        broadcastMessage("The topic has been changed to: " + topic, client);
    }
}

void Channel::setMode(ClientData* client, char mode, bool enable) {
    if (isOperator(client)) {
        // Implement mode setting logic here
    }
}

bool Channel::isOperator(ClientData* client) const {
    return operators_.find(client) != operators_.end();
}

void Channel::addOperator(ClientData* client) {
    operators_.insert(client);
}

void Channel::removeOperator(ClientData* client) {
    operators_.erase(client);
}

const std::string& Channel::getName() const {
    return name;
}