#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() : name("") {}

Channel::Channel(const std::string& name)
    : name(name), inviteOnly(false), topicRestricted(false), userLimit(0) {}

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

void Channel::kickClient(Client* client, Client* target) {
    if (isOperator(client)) {
        removeClient(target);
        target->sendMessage("You have been kicked from the channel: " + name);
    }
}

void Channel::inviteClient(Client* client, Client* target) {
    if (isOperator(client)) {
        addClient(target);
        target->sendMessage("You have been invited to the channel: " + name);
    }
}

void Channel::setTopic(Client* client, const std::string& topic) {
    if (!topicRestricted || isOperator(client)) {
        this->topic = topic;
        broadcastMessage("The topic has been changed to: " + topic, client);
    }
}

void Channel::setMode(Client* client, char mode, bool enable) {
    if (!isOperator(client)) return;

    switch (mode) {
        case 'i':
            inviteOnly = enable;
            break;
        case 't':
            topicRestricted = enable;
            break;
        case 'k':
            if (enable) {
                // パスワードを設定する場合は、追加の引数が必要
            } else {
                password.clear();
            }
            break;
        case 'o':
            if (enable) {
                operators_.insert(client);
            } else {
                operators_.erase(client);
            }
            break;
        case 'l':
            if (enable) {
                // ユーザー制限を設定する場合は、追加の引数が必要
            } else {
                userLimit = 0;
            }
            break;
        default:
            break;
    }
}

bool Channel::isOperator(Client* client) const {
    return operators_.find(client) != operators_.end();
}
