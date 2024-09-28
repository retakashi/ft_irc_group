#include "Channel.hpp"

#include <algorithm>

#include "ClientData.hpp"

Channel::Channel() {}

Channel::Channel(const std::string& name) : name(name), invite_only(false) {}

// void Channel::addClient(ClientData* client) { clients_.push_back(client); }

// void Channel::removeClient(ClientData* client) {
//   clients_.erase(std::remove(clients_.begin(), clients_.end(), client), clients_.end());
// }

// void Channel::broadcastMessage(const std::string& message, ClientData* sender) {
//   for (std::vector<ClientData*>::iterator it = clients_.begin(); it != clients_.end(); ++it) {
//     ClientData* client = *it;
//     if (client != sender) {
//       client->sendMessage(message);
//     }
//   }
// }

// void Channel::kickClient(ClientData* client, ClientData* target) {
//   if (isOperator(client)) {
//     removeClient(target);
//     target->sendMessage("You have been kicked from the channel: " + name);
//   }
// }

// void Channel::inviteClient(ClientData* client, ClientData* target) {
//   if (isOperator(client)) {
//     target->sendMessage("You have been invited to the channel: " + name);
//     // Add logic to actually add the client to the channel if needed
//   }
// }

// void Channel::setTopic(ClientData* client, const std::string& topic) {
//   if (isOperator(client)) {
//     this->topic = topic;
//     broadcastMessage("The topic has been changed to: " + topic, client);
//   }
// }

const std::string& Channel::getTopic() const { return topic; }

// const std::vector<ClientData*>& Channel::getClients() { return clients_; }

// void Channel::setMode(ClientData* client, char mode, bool enable) {
//   if (isOperator(client)) {
//     // Implement mode setting logic here
//   }
// }

// bool Channel::isOperator(ClientData* client) const {
//   return operators_.find(client) != operators_.end();
// }

// void Channel::addOperator(ClientData* client) { operators_.insert(client); }

// void Channel::removeOperator(ClientData* client) { operators_.erase(client); }

const std::string& Channel::getName() const { return name; }

void Channel::setInviteOnly(bool value) { invite_only = value; }
bool Channel::getInviteOnly() const { return invite_only; }
void Channel::setKey(const std::string& newkey) { key = newkey; }
const std::string& Channel::getKey() const { return key; }
void Channel::setTopicRestricted(bool value) { topic_restricted = value; }
bool Channel::getTopicRestricted() const { return topic_restricted; }
void Channel::setUserLimit(size_t limit){user_limit = limit; }
size_t Channel::getUserLimit() const{return user_limit;}
