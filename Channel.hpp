#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <set>
#include <string>
#include <vector>

#include "ClientData.hpp"  // ClientDataクラスを使うためにインクルード

class ClientData;
class Channel {
 public:
  Channel(const std::string& ch_name);
  ~Channel();

  void addClient(ClientData* client);
  void removeClient(ClientData* client);
  bool isOperator(ClientData* client) const;
  void broadcastMessage(const std::string& message, ClientData* sender = nullptr);

  void kickClient(ClientData* client, ClientData* target, const std::string& reason);
  void inviteClient(ClientData* client, ClientData* target);
  void setTopic(ClientData* client, const std::string& topic);
  const std::string& getTopic() const;
  const std::vector<ClientData*>& getClients() const;
  void setMode(ClientData* client, char mode, bool enable);
  //MODE.cpp
  bool toggleOperatorPrivileges(struct handle_mode_data data);
  void toggleInviteOnlyChannel(struct handle_mode_data data);
  bool toggleChannelKey(struct handle_mode_data data);
  void toggleTopicPrivileges(struct handle_mode_data data);
  bool toggleChannelLimit(struct handle_mode_data data);

 private:
  std::string ch_name_;
  std::string topic_;
  bool invite_only_;
  bool topic_restricted_;
  std::string key_;
  size_t user_limit_;
  std::vector<ClientData*> member_;
  std::vector<ClientData*> operators_;

  //↓rtakashi追加
  void setInviteOnly(bool value);
  bool getInviteOnly() const;
  const std::string& getName() const;
  // const std::string& getTopic() const;
  void setKey(const std::string& newkey);
  const std::string& getKey() const;
  void setTopicRestricted(bool value);
  bool getTopicRestricted() const;
  void setUserLimit(size_t limit);
  size_t getUserLimit() const;
  bool isMember(ClientData client) const;
  bool isValidKey(const std::string& key);
  void sendOtherMember(const std::string& str, ClientData me);
};

#endif // CHANNEL_HPP
