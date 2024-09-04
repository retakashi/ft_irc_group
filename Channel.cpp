#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() : name("") {}

Channel::Channel(const std::string& name) : name(name) {}

void Channel::addClient(Client* client) {
    clients_.push_back(client);
}

void Channel::removeClient(Client* client) {
    clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
}

void Channel::broadcastMessage(const std::string& message, Client* sender) {
    for (Client* client : clients_) {
        if (client != sender) {
            client->sendMessage(message);
        }
    }
}