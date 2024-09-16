#include "ClientData.hpp"

ClientData::ClientData() {}
ClientData::ClientData(int socket) : socket_(socket) {}
ClientData::~ClientData() {}

bool ClientData::isCompleteUserParams() {
  if (nickname.empty() || username.empty() || hostname.empty() || servername.empty() ||
      realname.empty())
    return false;
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

void ClientData::setNickname(const std::string& nickname) { this->nickname = nickname; }
void ClientData::setUsername(const std::string& username) { this->username = username; }
void ClientData::setHostname(const std::string& hostname) { this->hostname = hostname; }
void ClientData::setServername(const std::string& servername) { this->servername = servername; }
void ClientData::setRealname(const std::string& realname) { this->realname = realname; }

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

const std::string& ClientData::getNickname() const { return nickname; }
const std::string& ClientData::getUsername() const { return username; }
const std::string& ClientData::getHostname() const { return hostname; }
const int ClientData::getSocket() const { return socket_; }

void ClientData::sendMessage(const std::string& message) {
    ssize_t bytesSent = send(socket_, message.c_str(), message.size(), 0);
    if (bytesSent == -1) {
        std::cerr << "Error sending message: " << strerror(errno) << std::endl;
    }
}
// チャネル関連のメソッド
void ClientData::joinChannel(Channel& channel) {
  channel.addClient(this);
  sendMessage("Joined channel: " + channel.getName());
}

void ClientData::leaveChannel(Channel& channel) {
  channel.removeClient(this);
  sendMessage("Left channel: " + channel.getName());
}

void ClientData::sendChannelMessage(Channel& channel, const std::string& message) {
  channel.broadcastMessage(message, this);
}
