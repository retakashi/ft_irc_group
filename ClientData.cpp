#include "ClientData.hpp"

ClientData::ClientData() {}
ClientData::ClientData(int socket) : socket_(socket) {}
ClientData::~ClientData() {}

void ClientData::setNickname(const std::string& nickname) { nickname_ = nickname; }
void ClientData::setUsername(const std::string& username) { username_ = username; }
void ClientData::setHostname(const std::string& hostname) { hostname_ = hostname; }
void ClientData::setServername(const std::string& servername) { servername_ = servername; }
void ClientData::setRealname(const std::string& realname) { realname_ = realname; }

void ClientData::setUserParams(std::string params) {
  std::string user_data[4];
  std::string::size_type pos = 0;
  size_t i = 0;
  size_t j = 0;
  while (j < 4 && params[pos] != '\0') {
    pos = params.find(" ");
    if (j == 3 && params[0] == ':')
      user_data[real] = params;
    else {
      user_data[j] = params.substr(0, pos);
      user_data[j][pos] = '\0';
    }
    if (j < 3) {
      while (params[pos] != '\0' && params[pos] == ' ') pos++;
      if (params[pos] != '\0') params = params.substr(pos);
    }
    j++;
  }
  if (user_data[host].size() > 63) user_data[host].resize(63);
  if (user_data[server].size() > 63) user_data[server].resize(63);
  setUsername(user_data[user]);
  setHostname(user_data[host]);
  setServername(user_data[server]);
  setRealname(user_data[real]);
}

const std::string& ClientData::getNickname() const { return nickname_; }
const std::string& ClientData::getUsername() const { return username_; }
const std::string& ClientData::getHostname() const { return hostname_; }
const std::string& ClientData::getServername() const { return servername_; }
const std::string& ClientData::getRealname() const { return realname_; }
const int ClientData::getSocket() const { return socket_; }
