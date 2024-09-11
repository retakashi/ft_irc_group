#ifndef CLIENTDATA_HPP
#define CLIENTDATA_HPP

#include <cctype>

#include "Server.hpp"
#include "Channel.hpp"

class Server;

class ClientData {  // 一応hostname,servernameも入れておく。
 private:
  ClientData();
  int socket_;
  std::string nickname;  // 最大9文字だが、クライアントは将来的にプロトコルが進化する可能性を考慮して、より長い文字列を受け入れるべき->今回は20文字で
  std::string username;
  std::string hostname;  // 最大63文字。それ以上の場合、ホストアドレスを格納。->超えたら63字にリサイズ(9/8現在)
  std::string servername;  // 最大63文字　超えたら63字にリサイズ
  std::string realname;

 public:
  ClientData(int socket);
  ~ClientData();
  bool isCompleteUserParams();
  bool isUserParamCountValid(const std::string& params) const;
  void setNickname(const std::string& nickname);
  void setUsername(const std::string& username);
  void setHostname(const std::string& hostname);
  void setServername(const std::string& servername);
  void setRealname(const std::string& realname);
  void setUserParams(std::string params);
  const std::string& getNickname() const;
  const std::string& getUsername() const;
  const std::string& getHostname() const;
  const int getSocket() const;
  void sendMessage(const std::string& message);
    // チャネル関連のメソッド
  void joinChannel(Channel& channel);
  void leaveChannel(Channel& channel);
  void sendChannelMessage(Channel& channel, const std::string& message);

};

enum user_data { user, host, server, real };

#endif
