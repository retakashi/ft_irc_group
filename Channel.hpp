#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ClientData.hpp"  // ClientDataクラスを使うためにインクルード

class ClientData;
class Channel {
 public:
  Channel(const std::string& ch_name);
  ~Channel();

  //channel.cpp
  void broadcastMessage(const std::string& message, ClientData* sender);
  void addClient(ClientData* client);
  void addOperator(ClientData* client);
  bool isMember(ClientData* client) const;
  size_t CountMember() const;
  //getter
  const std::vector<ClientData*>& getClients() const;
  const std::string& getChannelname() const;
  const std::string& getTopic() const;
  bool getInviteOnly() const;
  bool getTopicRestricted() const;
  const std::string& getKey() const;
  size_t getUserLimit() const;
  ClientData* getMemberByNickname(const std::string& nickname);
  //Channel.cpp 他で使用なければMODE.cppに移すかも?
  void setInviteOnly(bool value);
  void setKey(const std::string& newkey);
  void setTopicRestricted(bool value);
  void setUserLimit(size_t limit);
  void setTopic(const std::string& topic);
  //JOIN.cpp
  std::string getMemberList() const; //add
  //MODE.cpp
  bool toggleOperatorPrivileges(struct handle_mode_data& data);
  void toggleInviteOnlyChannel(struct handle_mode_data data);
  bool toggleChannelKey(struct handle_mode_data& data);
  bool isValidKey(const std::string& key);
  void toggleTopicPrivileges(struct handle_mode_data data);
  bool toggleChannelLimit(struct handle_mode_data& data);
  size_t convertStringToUserLimit(const std::string& l_param);
  // INVITE.cpp
  void  inviteMember(ClientData* client, ClientData* target);
  // KICK.cpp
  void removeClient(ClientData* client);
  bool isOperator(ClientData* client) const;
  void kickMember(ClientData* client, ClientData* target, const std::string& reason);

 private:
  std::string ch_name_;
  std::string topic_;
  bool invite_only_; //operator権限なのかどうか MODE +/-i
  bool topic_restricted_; //operator権限なのかどうか MODE +/-t
  std::string key_; //設定されていたらJOIN時に使用しなければならない
  size_t user_limit_;
  std::vector<ClientData*> member_;
  std::vector<ClientData*> operators_;
};

#endif