#include "Channel.hpp"
#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Server.hpp"

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
  broadcastMessage(client->getNickname() + " has joined the channel.",client);
}

void Channel::removeClient(ClientData* client) {
  member_.erase(std::remove(member_.begin(), member_.end(), client), member_.end());
  broadcastMessage(client->getNickname() + " has left the channel.", client);
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

void Channel::inviteMember(ClientData* client, ClientData* target) {
  if (isOperator(client)) {
    std::string inviteMsg = createCmdRespMsg(Server::servername_, RPL_CHANNELMODEIS,
                                             "INVITE " + target->getNickname() + " :" + ch_name_);
    Server::ft_send(inviteMsg, *target);
    // Add logic to actually add the client to the channel if needed
  }
}

bool Channel::isMember(ClientData* client) const {
    // Loop through the member list to check if the client is in the channel
    for (std::vector<ClientData*>::const_iterator it = member_.begin(); it != member_.end(); ++it) {
        if (*it == client) {
            return true;
        }
    }
    return false;  // Client is not a mesmber of the channel
}

//gettter
const std::vector<ClientData*>& Channel::getClients() const { return member_; }
const std::string& Channel::getChannelname() const { return ch_name_; }
const std::string& Channel::getTopic() const { return topic_; }
bool Channel::getInviteOnly() const { return invite_only_; }
bool Channel::getTopicRestricted() const { return topic_restricted_; }
const std::string& Channel::getKey() const { return key_; }
size_t Channel::getUserLimit() const { return user_limit_; }
ClientData* Channel::getMemberByNickname(const std::string& nickname) {
  for (size_t i = 0; i < member_.size(); i++)
    if (member_[i]->getNickname() == nickname) return member_[i];
  return NULL;
}

// ↓rtakashi追加 MODEでしか使わないものはMODE.cppに移します
void Channel::setInviteOnly(bool value) { invite_only_ = value; }
void Channel::setKey(const std::string& newkey) { key_ = newkey; }
void Channel::setTopicRestricted(bool value) { topic_restricted_ = value; }
void Channel::setUserLimit(size_t limit) { user_limit_ = limit; }
void Channel::setTopic(const std::string& topic) { topic_ = topic; }
