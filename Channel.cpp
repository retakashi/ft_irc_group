#include "Channel.hpp"
#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Server.hpp"
#include <algorithm>

Channel::Channel(const std::string& name) : ch_name_(name) {}

Channel::~Channel() {
    // クライアントリストをクリア
    member_.clear();
    // オペレータリストをクリア
    operators_.clear();
}

void Channel::broadcastMessage(const std::string& message, ClientData* sender) {
    for (std::vector<ClientData*>::iterator it = member_.begin(); it != member_.end(); ++it) {
        if (*it != sender) {
            Server::ft_send(message, **it);
        }
    }
}

void Channel::addClient(ClientData* client) {
    member_.push_back(client);
    broadcastMessage(client->getNickname() + " has joined the channel.");
}

void Channel::removeClient(ClientData* client) {
    member_.erase(std::remove(member_.begin(), member_.end(), client), member_.end());
    broadcastMessage(client->getNickname() + " has left the channel.");
}

bool Channel::isOperator(ClientData* client) const {
    return std::find(operators_.begin(), operators_.end(), client) != operators_.end();
}

void Channel::kickClient(ClientData* client, ClientData* target, const std::string& reason) {
    if (isOperator(client)) {
        removeClient(target);
        std::string kickMsg = createCmdRespMsg(Server::servername_, ERR_USERNOTINCHANNEL, "KICK " + ch_name_ + " " + target->getNickname() + " :" + reason);
        Server::ft_send(kickMsg, *target);
    }
}

void Channel::inviteClient(ClientData* client, ClientData* target) {
    if (isOperator(client)) {
        std::string inviteMsg = createCmdRespMsg(Server::servername_, RPL_CHANNELMODEIS, "INVITE " + target->getNickname() + " :" + ch_name_);
        Server::ft_send(inviteMsg, *target);
        // Add logic to actually add the client to the channel if needed
    }
}

void Channel::setTopic(ClientData* client, const std::string& topic) {
    if (isOperator(client)) {
        topic_ = topic;
        std::string topicMsg = createCmdRespMsg(Server::servername_, RPL_CHANNELMODEIS, "TOPIC " + ch_name_ + " :" + topic);
        broadcastMessage(topicMsg, client);
    }
}

const std::string& Channel::getTopic() const { return topic_; }

const std::vector<ClientData*>& Channel::getClients() const { return member_; }

void Channel::setMode(ClientData* client, char mode, bool enable) {
    if (isOperator(client)) {
        // Implement mode setting logic here
    }
}
//↓rtakashi追加 MODEでしか使わないものはMODE.cppに移します
void Channel::setInviteOnly(bool value) { invite_only_ = value; }
bool Channel::getInviteOnly() const { return invite_only_; }
void Channel::setKey(const std::string& newkey) { key_ = newkey; }
const std::string& Channel::getKey() const { return key_; }
void Channel::setTopicRestricted(bool value) { topic_restricted_ = value; }
bool Channel::getTopicRestricted() const { return topic_restricted_; }
void Channel::setUserLimit(size_t limit){user_limit_ = limit; }
size_t Channel::getUserLimit() const{return user_limit_;}
