#include "ClientData.hpp"

ClientData::ClientData() {}
ClientData::ClientData(int socket) : socket_(socket),mode_('\0') {}
ClientData::~ClientData() {}

void ClientData::setNickname(const std::string& nickname) { nickname_ = nickname; }
void ClientData::setUsername(const std::string& username) { username_ = username; }
void ClientData::setMode(const char mode) { mode_ = mode; }
void ClientData::setRealname(const std::string& realname) { realname_ = realname; }

const std::string& ClientData::getNickname() const { return nickname_; }
const std::string& ClientData::getUsername() const { return username_; }
const std::string& ClientData::getRealname() const { return realname_; }
const int ClientData::getSocket() const { return socket_; }
