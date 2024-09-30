#include "ClientData.hpp"

ClientData::ClientData() {}
ClientData::ClientData(int socket) : socket_(socket), mode_('\0'), is_authenticated(false) {}
ClientData::~ClientData() {}

bool ClientData::isCompleteAuthParams() {
  if (!is_authenticated || nickname_.empty() || realname_.empty()) return false;
  return true;
}

void ClientData::setAuth(bool auth) { is_authenticated = auth; }
void ClientData::setNickname(const std::string& nickname) {
  std::stringstream ss;
  ss << "You set nickname " << getNickname();
  Server::ft_send(ss.str(), *this);
  nickname_ = nickname;
}
void ClientData::setUsername(const std::string& username) { username_ = username; }
void ClientData::setMode(const char mode) { mode_ = mode; }
void ClientData::setRealname(const std::string& realname) { realname_ = realname; }
void ClientData::setSocket(const int socket) { socket_ = socket; }
const bool ClientData::getAuth() const { return is_authenticated; }
const std::string& ClientData::getNickname() const { return nickname_; }
const std::string& ClientData::getUsername() const { return username_; }
const std::string& ClientData::getRealname() const { return realname_; }
const int ClientData::getSocket() const { return socket_; }
