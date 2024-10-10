#include "Channel.hpp"

#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Server.hpp"

Channel::Channel(const std::string& name)
    : ch_name_(name), invite_only_(false), topic_restricted_(false), user_limit_(0) {}

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

void Channel::addMember(ClientData* client) {
  member_.push_back(client);
  broadcastMessage(client->getNickname() + " has joined the channel.", client);
}

void Channel::addOperator(ClientData* client) {
  operators_.push_back(client);
  // broadcastMessage(client->getNickname() + " has joined the channel.",client);
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

size_t Channel::CountMember() const { return member_.size(); }

// gettter
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
