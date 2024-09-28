#include "../Server.hpp"

/*
MODE <channel> <option> <param>
+/-o: param(nick)にオペレーション権限を与える/奪う
+/-i: channelを招待のみに制限/解除.modeparam不要
+/-k: channelにJOINする際にkeyを必要にする/解除
+/-t: TOPICコマンドの権限をオペレータのみ/解除.modeparam不要
+/-l: ユーザー制限(チャンネルに参加できる人数)/解除
*/
int Server::handleMODE(std::string param, ClientData& client) {
  std::vector<std::string> mode_data;
  std::string channelname;
  Channel* channel;
  bool is_active = false;
  size_t start = 0;
  size_t next = 0;
  size_t i = 0;
  if (searchChannel(param, channelname, client) == false) return 0;
  channel = channels_[channelname];
  //  if (channel.operators_.find(client.getNickname()) == channel.operators_.end())
  //     return sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelname, client);
  setModeData(param, mode_data);
  if (isValidModeData(mode_data, client, *channel) == false) return 0;
  while (start < mode_data.size()) {
    i = 0;
    while (mode_data[start][i] != '\0') {
      switch (mode_data[start][i]) {
        case '+':
          is_active = true;
          break;
        case '-':
          is_active = false;
          break;
        case 'o':
          next++;
          if (toggleOperatorPrivileges(mode_data, is_active, channel, client) == false) return 0;
          break;
        case 'i':
          toggleInviteOnlyChannel(is_active, channel, client);
          break;
        case 'k':
        next++;
          if (toggleChannelKey(mode_data, is_active, channel, client) == false) return 0;
          break;
        case 't':
          toggleTopicPrivileges(is_active, channel, client);
          break;
          case 'l':
          next++;
            if (toggleChannelLimit(mode_data, is_active,channel, client) == false) return 0;
            break;
          // default:
          //   sendCmdResponce();  // 無効なmode
      }
      i++;
    }
    start = next + 1;
  }
  return 0;
}

// paramからmode,modeoptionを切り出すのも行う
bool Server::searchChannel(std::string& param, std::string& channelname, ClientData client) {
  if (param.empty()) return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);  // false返す
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
  channelname = param.substr(0, pos);
  channelname[pos] = '\0';
  std::map<std::string, Channel*>::iterator it = channels_.find(channelname);
  if (it == channels_.end()) return sendCmdResponce(ERR_NOSUCHCHANNEL, channelname, client);
  param = param.substr(pos + 1);
  return true;
}

void Server::setModeData(std::string& param, std::vector<std::string>& mode_data) {
  std::string mode;

  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos)
    mode_data.push_back(param);
  else {
    while (param.size() > 0) {
      pos = param.find(' ');
      if (pos == std::string::npos) {
        mode_data.push_back(param);
        break;
      }
      mode = param.substr(0, pos);
      mode[pos] = '\0';
      mode_data.push_back(mode);
      param = param.substr(pos + 1);
    }
  }
}

bool Server::isValidMode(const std::string& mode, int& total_cnt, int& need_cnt, ClientData client,
                         Channel channel) {
  std::string mode_str = "oiktl";
  if (mode[0] != '+' && mode[0] != '-')
    return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);  // false返す
  for (size_t i = 1; mode[i] != '\0' && mode[i] != ' '; i++) {
    if (mode_str.find(mode[i]) != std::string::npos)
      total_cnt++;
    else
      return sendCmdResponce(ERR_NOCHANMODES, channel.getName(), client);
    if (mode[i] == 'o' || mode[i] == 'k' || mode[i] == 'l') need_cnt++;
  }
  if (total_cnt < 1 || total_cnt > 2)
    return sendCmdResponce(ERR_NOCHANMODES, channel.getName(), client);
  return true;
}

bool Server::isValidModeData(const std::vector<std::string>& mode_data, ClientData client,
                             Channel channel) {
  int total_cnt = 0;
  int need_cnt = 0;
  int start = 0;
  while (start < mode_data.size()) {
    need_cnt = 0;
    if (isValidMode(mode_data[start], total_cnt, need_cnt, client, channel) == false) return false;
    start += (need_cnt + 1);
    if (mode_data.size() == start) return true;
    if (mode_data.size() < start) return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
  }
  return true;
}

// 使用したmode's paramがあれば消す
bool Server::toggleOperatorPrivileges(std::vector<std::string> mode_data, bool is_active,
                                      Channel* channel, const ClientData& client) {
  std::string target_nick;
  std::map<std::string, ClientData*>::iterator member;
  std::map<std::string, ClientData*>::iterator ope;
  std::stringstream ss;

  target_nick = mode_data[1];
  member = channel->member_.find(target_nick);
  if (member == channel->member_.end())
    return sendCmdResponce(ERR_USERNOTINCHANNEL, target_nick, "MODE", client);
  ope = channel->operators_.find(target_nick);
  if (is_active == true) {
    if (target_nick == client.getNickname())
      return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    if (ope == channel->operators_.end())
      channel->operators_.insert(std::make_pair(target_nick, member->second));
    ss << channel->getName() << " +o by " << client.getNickname();
    sendCmdResponce(RPL_CHANNELMODEIS, ss.str(), *channel->operators_[target_nick]);
  } else if (is_active == false && ope != channel->operators_.end()) {
    if (target_nick != client.getNickname()) {
      ss << channel->getName() << " -o by " << client.getNickname();
      sendCmdResponce(RPL_CHANNELMODEIS, ss.str(), *channel->operators_[target_nick]);
    }
    if (channel->operators_.size() > 0) channel->operators_.erase(member);
  }
  // mode_data.erase(mode_data.begin() + 1);
  return true;
}

void Server::toggleInviteOnlyChannel(bool is_active, Channel* channel, const ClientData& client) {
  std::stringstream ss;
  if (is_active == true) {
    channel->setInviteOnly(true);
    ss << channel->getName() << " +i by " << client.getNickname();
  } else {
    channel->setInviteOnly(false);
    ss << channel->getName() << " -i by " << client.getNickname();
  }
  sendOtherMember(ss.str(), *channel, client);
}

bool Server::toggleChannelKey(std::vector<std::string>& mode_data, bool is_active, Channel* channel,
                              const ClientData& client) {
  std::stringstream ss;
  if (is_active == true) {
    if (!channel->getKey().empty()) return sendCmdResponce(ERR_KEYSET, channel->getName(), client);
    if (isValidKey(mode_data[1]) == false)
      return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    channel->setKey(mode_data[1]);
    ss << channel->getName() << " +k " << channel->getKey() << " by " << client.getNickname();
  } else {
    if (!channel->getKey().empty()) channel->setKey("");
    ss << channel->getName() << " -k " << " by " << client.getNickname();
  }
  sendOtherMember(ss.str(), *channel, client);
  // mode_data.erase(mode_data.begin() + 1);
  return true;
}

bool Server::isValidKey(const std::string& key) {
  if (key.size() < 1 || key.size() > 23) return false;
  for (size_t i = 0; i < key.size(); i++) {
    if (std::isspace(key[i])) return false;
  }
  return true;
}

void Server::toggleTopicPrivileges(bool is_active, Channel* channel, const ClientData& client) {
  std::stringstream ss;
  if (is_active == true) {
    channel->setTopicRestricted(true);
    ss << channel->getName() << " +t by " << client.getNickname();
  } else {
    channel->setTopicRestricted(false);
    ss << channel->getName() << " -t by " << client.getNickname();
  }
  sendOtherMember(ss.str(), *channel, client);
}

// limit -> とりあえずINT_MAXまで
#define INT_MAX_LEN 10
bool Server::toggleChannelLimit(std::vector<std::string>& mode_data, bool is_active,
                                Channel* channel, ClientData client) {
  size_t limit = 0;
  std::stringstream ss;

  if (is_active == true) {
    if (mode_data[1].size() > INT_MAX_LEN) return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);;
    ss << mode_data[1];
    ss >> limit;
    if (limit > INT_MAX) return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);;
    channel->setUserLimit(limit);
    ss.clear();
    ss << channel->getName() << " " << limit << " +l by " << client.getNickname();
  } else {
    if (channel->getUserLimit() > 0) {
      channel->setUserLimit(0);
      ss << channel->getName() << " -l by " << client.getNickname();
    }
  }
  sendOtherMember(ss.str(), *channel, client);
  return true;
  // mode_data.erase(mode_data.begin() + 1);
}

void Server::sendOtherMember(const std::string str, Channel channel, ClientData me) {
  std::map<std::string, ClientData*>::iterator member_it = channel.member_.begin();
  while (member_it != channel.member_.end()) {
    if (member_it->first != me.getNickname())
      sendCmdResponce(RPL_CHANNELMODEIS, str, *member_it->second);
    member_it++;
  }
}
