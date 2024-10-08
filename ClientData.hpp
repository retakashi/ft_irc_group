#ifndef CLIENTDATA_HPP
#define CLIENTDATA_HPP

#include <cctype>

#include "Server.hpp"

class Server;

class ClientData {
 private:
  ClientData();
  int socket_;
  char mode_;
  bool        is_authenticated;
  std::string nickname_; //最大20文字
  std::string username_;//最大20文字
  std::string realname_;//最大100文字
 public:
  ClientData(int socket);
  ~ClientData();
  bool isCompleteAuthParams();
  void setAuth(bool auth);
  void setNickname(const std::string& nickname);
  void setUsername(const std::string& username);
  void setMode(const char mode);
  void setRealname(const std::string& realname);
  void setSocket(const int socket);
  const bool          getAuth() const;
  const std::string& getNickname() const;
  const std::string& getUsername() const;
  const std::string& getRealname() const;
  const int getSocket() const;
};
#endif
