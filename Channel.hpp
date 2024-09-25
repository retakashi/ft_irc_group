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
  // const std::string& getName() const;
  // const std::string& getTopic() const;           // トピックを取得するメソッド
  // const std::vector<ClientData*>& getClients();  // クライアントリストを取得するメソッド
                                                 // チャネル関連のメソッド

 private:
  std::string name; // チャンネル名
  std::string topic;
  std::vector<ClientData*> clients_;
  std::set<ClientData*> operators_;
  bool inviteOnly;  // add↓
  bool topicRestricted;
  std::string password;
  size_t userLimit;
};

#endif  // CHANNEL_HPP
