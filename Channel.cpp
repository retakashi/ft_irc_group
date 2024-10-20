#include "Channel.hpp"

#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Server.hpp"

Channel::Channel(const std::string& name)
    : ch_name_(name), invite_only_(false), topic_restricted_(false), user_limit_(0) {}

Channel::~Channel() {}

void Channel::addMember(ClientData* client) { members_.push_back(client); }

void Channel::addOperator(ClientData* client) { operators_.push_back(client); }

bool Channel::isMember(ClientData* client) const {
  return std::find(members_.begin(), members_.end(), client) != members_.end();
}

bool Channel::isOperator(ClientData* client) const {
  return std::find(operators_.begin(), operators_.end(), client) != operators_.end();
}

bool Channel::isInvitee(ClientData* client) const {
  return std::find(invitees_.begin(), invitees_.end(), client) != invitees_.end();
}

void Channel::removeMember(ClientData* client) {
  if (client != NULL)
    members_.erase(std::remove(members_.begin(), members_.end(), client), members_.end());
}

void Channel::removeOperator(ClientData* client) {
  if (client != NULL)
    operators_.erase(std::remove(operators_.begin(), operators_.end(), client), operators_.end());
}

void Channel::removeInvitee(ClientData* client) {
  if (client != NULL)
    invitees_.erase(std::remove(invitees_.begin(), invitees_.end(), client), invitees_.end());
}

size_t Channel::CountMembers() const { return members_.size() + operators_.size(); }

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
Channel* Server::getChannelByName(const std::string& channelName) {
  std::map<std::string, Channel*>::iterator it = channels_.find(channelName);
  if (it != channels_.end()) {
    return it->second;
  }
  return NULL;
}
//setter
void Channel::setInviteOnly(bool value) { invite_only_ = value; }
void Channel::setKey(const std::string& newkey) { key_ = newkey; }
void Channel::setTopicRestricted(bool value) { topic_restricted_ = value; }
void Channel::setUserLimit(size_t limit) { user_limit_ = limit; }
void Channel::setTopic(const std::string& topic) { topic_ = topic; }
