#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

class ClientData;

class Channel {
 public:
  Channel();
  Channel(const std::string& name);
  std::vector<ClientData*> member_;
  std::vector<ClientData*> operators_;
  // void  addClient(ClientData* client);
  // void  removeClient(ClientData* client);
  // void  broadcastMessage(const std::string& message, ClientData* sender);
  // void  kickClient(ClientData* client, ClientData* target);  // add↓
  // void  inviteClient(ClientData* client, ClientData* target);
  // void  setTopic(ClientData* client, const std::string& topic);
  // void  setMode(ClientData* client, char mode, bool enable);
  // bool  isOperator(ClientData* client) const;
  // void  addOperator(ClientData* client);
  // void  removeOperator(ClientData* client);
  void setInviteOnly(bool value);
  bool getInviteOnly() const;
  const std::string& getName() const;
  const std::string& getTopic() const;
  void setKey(const std::string& newkey);
  const std::string& getKey() const;
  void setTopicRestricted(bool value);
  bool getTopicRestricted() const;
  void setUserLimit(size_t limit);
  size_t getUserLimit() const;
  bool isMember(ClientData client) const;
  //MODE.cpp
  bool toggleOperatorPrivileges(struct handle_mode_data data);
  void toggleInviteOnlyChannel(struct handle_mode_data data);
  bool toggleChannelKey(struct handle_mode_data data);
  bool isValidKey(const std::string &key);
  void toggleTopicPrivileges(struct handle_mode_data data);
  bool toggleChannelLimit(struct handle_mode_data data);
  void sendOtherMember(const std::string& str, ClientData me);
  // クライアントリストを取得するメソッド チャネル関連のメソッド

 private:
  std::string name;  // チャンネル名
  std::string topic;
  bool invite_only;  // add↓
  bool topic_restricted;
  std::string key;
  size_t user_limit;
};

#endif 
