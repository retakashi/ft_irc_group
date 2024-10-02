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

void Channel::kickMember(ClientData* client, ClientData* target, const std::string& reason) {
    if (isOperator(client)) {
        std::vector<ClientData*>::iterator it = std::find(member_.begin(), member_.end(), target);
        if (it != member_.end()) {
            removeClient(target);
            std::string message = target->getNickname() + " has been kicked from the channel. Reason: ";
            broadcastMessage(message, client);
        } else {
            std::cerr << "Target client not found in the channel." << std::endl;
        }
    } else {
        std::cerr << "Client is not an operator." << std::endl;
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

bool Channel::isMember(ClientData* client) const {
    // Loop through the member list to check if the client is in the channel
    for (std::vector<ClientData*>::const_iterator it = member_.begin(); it != member_.end(); ++it) {
        if (*it == client) {
            return true;
        }
    }
    return false;  // Client is not a mesmber of the channel
}
