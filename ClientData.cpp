#include "ClientData.hpp"

ClientData::ClientData() {}
ClientData::ClientData(int socket) : socket_(socket) {}
ClientData::~ClientData() {}

bool ClientData::isCompleteUserParams() {
  if (nickname_.empty() || username_.empty() || hostname_.empty() || servername_.empty() ||
      realname_.empty())
    return false;
  return true;
}

// nicknameは最大20字まで
bool ClientData::isValidNickname(const std::string& param) const {
  if (param.size() > 20 || param.size() < 1) return false;
  if (!std::isalpha(param[0])) return false;
  size_t i = 0;
  std::string special = "-[]\\`^{}";
  while (param[i] != '\0') {
    if (!std::isalnum(param[i]) && special.find(param[i]) == std::string::npos) return false;
    i++;
  }
  return true;
}

/* realname　':'の後に任意のオクテット列（バイト列）で、SPACE、NUL、CR、CF
を含むことはできない。->エラーにする */
bool ClientData::isUserParamCountValid(const std::string& params) const {
  size_t space_cnt = 0;
  size_t i = 0;
  while (params[i] != '\0') {
    if (params[i] == ' ') {
      space_cnt++;
      while (params[i] == ' ') i++;
      if (params[i] == '\0') break;
    }
    if (params[i] == ':' && (params[i + 1] == '\0' || params[i + 1] == '\r' ||
                             params[i + 1] == '\n' || params[i + 1] == ' '))
      return false;
    i++;
  }
  if (space_cnt < 3) return false;
  return true;
}

void ClientData::setNickname(const std::string& nickname) { nickname_ = nickname; }
void ClientData::setUsername(const std::string& username) { username_ = username; }
void ClientData::setHostname(const std::string& hostname) { hostname_ = hostname; }
void ClientData::setServername(const std::string& servername) { servername_ = servername; }
void ClientData::setRealname(const std::string& realname) { realname_ = realname; }

void ClientData::setUserParams(std::string params) {
  std::string user_data[4];
  std::string::size_type pos = 0;
  size_t j = 0;
  while (j < 4) {
    pos = params.find(" ");
    if (j == 3 && params[0] == ':')
      user_data[real] = params;
    else
    {
      user_data[j] = params.substr(0, pos);
      user_data[j][pos] = '\0';
    }
    params = params.substr(pos + 1);
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
const int ClientData::getSocket() const { return socket_; }
