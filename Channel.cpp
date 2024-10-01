#include "Channel.hpp"
#include "Server.hpp"
#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include <algorithm>

class ClientData;
class Server;

Channel::Channel(const std::string& name) : name_(name) {}

Channel::~Channel() {
  // クライアントのリストをクリア
  clients_.clear();
  // オペレータのリストをクリア
  operators_.clear();
}

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
        std::string kickMsg = createCmdRespMsg(client->getServername(), "KICK " + name_ + " " + target->getNickname() + " :" + reason);
        ft_send(*target, createSendMsg(kickMsg));
    }
}

void Channel::inviteClient(ClientData* client, ClientData* target) {
    if (isOperator(client)) {
        std::string inviteMsg = createCmdRespMsg(client->getServername(), "INVITE " + target->getNickname() + " :" + name_);
        ft_send(*target, createSendMsg(inviteMsg));
        // Add logic to actually add the client to the channel if needed
    }
}

void Channel::setTopic(ClientData* client, const std::string& topic) {
    if (isOperator(client)) {
        topic_ = topic;
        std::string topicMsg = createCmdRespMsg(client->getServername(), "TOPIC " + name_ + " :" + topic);
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

//↓rtakashi追加 MODEでしか使わないものはMODE.cppに移します
void Channel::setInviteOnly(bool value) { invite_only_ = value; }
bool Channel::getInviteOnly() const { return invite_only_; }
void Channel::setKey(const std::string& newkey) { key_ = newkey; }
const std::string& Channel::getKey() const { return key_; }
void Channel::setTopicRestricted(bool value) { topic_restricted_ = value; }
bool Channel::getTopicRestricted() const { return topic_restricted_; }
void Channel::setUserLimit(size_t limit){user_limit_ = limit; }
size_t Channel::getUserLimit() const{return user_limit_;}
