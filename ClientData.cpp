#include "ClientData.hpp"

ClientData::ClientData() {}
ClientData::ClientData(int socket) : socket_(socket),mode_('\0') {}
ClientData::~ClientData() {}

bool ClientData::isCompleteAuthParams() {
  if (nickname_.empty() || realname_.empty()) return false;
  return true;
}

void ClientData::setNickname(const std::string& nickname) { nickname_ = nickname; }
void ClientData::setUsername(const std::string& username) { username_ = username; }
void ClientData::setMode(const char mode) { mode_ = mode; }
void ClientData::setRealname(const std::string& realname) { realname_ = realname; }
void ClientData::setSocket(const int socket) {socket_ = socket; }
const std::string& ClientData::getNickname() const { return nickname_; }
const std::string& ClientData::getUsername() const { return username_; }
const std::string& ClientData::getRealname() const { return realname_; }
const int ClientData::getSocket() const { return socket_; }
