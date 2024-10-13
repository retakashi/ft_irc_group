#include "Channel.hpp"

#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Server.hpp"

Channel::Channel(const std::string& name)
    : ch_name_(name), invite_only_(false), topic_restricted_(false), user_limit_(0) {}

Channel::~Channel() {}

void Channel::broadcastMessage(const std::string& message, ClientData* sender) {
  for (std::vector<ClientData*>::iterator it = members_.begin(); it != members_.end(); ++it) {
    if (*it != sender) {
      Server::ft_send(message, **it);
    }
  }
}

void Channel::addMember(ClientData* client) {
  members_.push_back(client);
  broadcastMessage(client->getNickname() + " has joined the channel.", client);
}

void Channel::addOperator(ClientData* client) {
  operators_.push_back(client);
  // broadcastMessage(client->getNickname() + " has joined the channel.",client);
}

bool Channel::isMember(ClientData* client) const {
  // Loop through the member list to check if the client is in the channel
  for (std::vector<ClientData*>::const_iterator it = members_.begin(); it != members_.end(); ++it) {
    if (*it == client) {
      return true;
    }
  }
  return false;  // Client is not a mesmber of the channel
}

bool Channel::isOperator(ClientData* client) const {
  return std::find(operators_.begin(), operators_.end(), client) != operators_.end();
}

void Channel::removeMember(ClientData* client) {
  if (client != NULL)
    members_.erase(std::remove(members_.begin(), members_.end(), client), members_.end());
}

void Channel::removeOperator(ClientData* client) {
  if (client != NULL)
    operators_.erase(std::remove(operators_.begin(), operators_.end(), client), operators_.end());
}

size_t Channel::CountMember() const { return members_.size(); }

// gettter
const std::vector<ClientData*>& Channel::getMembers() const { return members_; }
const std::string& Channel::getChannelname() const { return ch_name_; }
const std::string& Channel::getTopic() const { return topic_; }
bool Channel::getInviteOnly() const { return invite_only_; }
bool Channel::getTopicRestricted() const { return topic_restricted_; }
const std::string& Channel::getKey() const { return key_; }
size_t Channel::getUserLimit() const { return user_limit_; }
ClientData* Channel::getMemberByNickname(const std::string& nickname) {
  for (size_t i = 0; i < members_.size(); i++)
    if (members_[i]->getNickname() == nickname) return members_[i];
  return NULL;
}
ClientData* Channel::getOperatorByNickname(const std::string& nickname) {
  for (size_t i = 0; i < operators_.size(); i++)
    if (operators_[i]->getNickname() == nickname) return operators_[i];
  return NULL;
}

// ↓rtakashi追加 MODEでしか使わないものはMODE.cppに移します
void Channel::setInviteOnly(bool value) { invite_only_ = value; }
void Channel::setKey(const std::string& newkey) { key_ = newkey; }
void Channel::setTopicRestricted(bool value) { topic_restricted_ = value; }
void Channel::setUserLimit(size_t limit) { user_limit_ = limit; }
void Channel::setTopic(const std::string& topic) { topic_ = topic; }
