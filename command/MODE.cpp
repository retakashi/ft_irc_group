#include "Channel.hpp"
#include "Server.hpp"

/*
MODE <channel> <option> <param>
+/-o: param(nick)にオペレーション権限を与える/奪う
+/-i: channelを招待のみに制限/解除.modeparam不要
+/-k: channelにJOINする際にkeyを必要にする/解除
+/-t: TOPICコマンドの権限をオペレータのみ/解除.modeparam不要
+/-l: ユーザー制限(チャンネルに参加できる人数)/解除
*/
int Server::handleMode(std::string param, ClientData& client) {
  handle_mode_data data(client);
  size_t start = 1;
  Channel* ch;
  char mode;
  std::string send_mode;

  if (splitChannelNameAndMode(param, data) == false) return 0;
  ch = channels_[data.mode_data[0]];
  if (param.empty())  // MODE #chの場合
  {
    std::string mode_status = ch->getModeStatus();
    return sendCmdResponce(RPL_CHANNELMODEIS, ch->getChannelname(), mode_status, client);
  }
  if (ch->isOperator(&client) == false)
    return Server::sendCmdResponce(ERR_CHANOPRIVSNEEDED, data.mode_data[0], data.client);
  splitModeParam(param, data.mode_data);
  if (isValidModeData(data) == false) return 0;
  data.param_i = start;
  while (start < data.mode_data.size()) {
    for (size_t i = 0; data.mode_data[start][i] != '\0'; i++) {
      mode = data.mode_data[start][i];
      switch (mode) {
        case '+':
          data.is_active = true;
          break;
        case '-':
          data.is_active = false;
          break;
        case 'o':
          if (ch->toggleOperatorPrivileges(data) == false) return 0;
          break;
        case 'i':
          ch->toggleInviteOnlyChannel(data);
          break;
        case 'k':
          if (ch->toggleChannelKey(data) == false) return 0;
          break;
        case 't':
          ch->toggleTopicPrivileges(data);
          break;
        case 'l':
          if (ch->toggleChannelLimit(data) == false) return 0;
          break;
        default:
          return Server::sendCmdResponce(ERR_NOCHANMODES, data.mode_data[0], data.client);
      }
    }
    if (start >= data.param_i)
      start++;
    else
      start = data.param_i + 1;
    data.param_i = start;
  }
  std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + " MODE " +
                    ch->getChannelname() + " " + param;
  ch->sendAll(msg);
  return 0;
}

// paramからchannelnameを切り、mode,mode's paramのみにする
bool Server::splitChannelNameAndMode(std::string& param, struct handle_mode_data& data) {
  std::string ch_name;
  if (param.empty())
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);  // false返す
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) {
    ch_name = param;
    param.clear();
  } else {
    ch_name = param.substr(0, pos);
    ch_name[pos] = '\0';
    param = param.substr(pos + 1);
  }
  std::map<std::string, Channel*>::iterator it = Server::channels_.find(ch_name);
  if (it == Server::channels_.end())
    return Server::sendCmdResponce(ERR_NOSUCHCHANNEL, ch_name, data.client);
  data.mode_data.push_back(ch_name);
  return true;
}

std::string Channel::getModeStatus() {
  std::string mode_status;
  if (operators_.size() > 0) {
    mode_status += " +o ";
    for (size_t i = 0; i < operators_.size(); i++) {
      mode_status += operators_[i]->getNickname();
      if (i != operators_.size() - 1) mode_status += " ";
    }
  }
  if (getInviteOnly() == true) mode_status += " +i";
  if (getTopicRestricted() == true) mode_status += " +t";
  if (!getKey().empty()) mode_status += " +k " + getKey();
  if (getUserLimit() > 0) mode_status += "+l " + getUserLimit();
  return mode_status;
}

// paramをsplitしてmode_dataに格納
void Server::splitModeParam(std::string param, std::vector<std::string>& mode_data) {
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

bool Server::isValidModeData(struct handle_mode_data& data) {
  int total_cnt = 0;
  int need_cnt = 0;
  int start = 1;
  while (start < data.mode_data.size()) {
    need_cnt = 0;
    if (isValidMode(data, start, total_cnt, need_cnt) == false) return false;
    start += (need_cnt + 1);
    if (data.mode_data.size() == start) return true;
    if (data.mode_data.size() < start + 1)
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
  }
  return true;
}

bool Server::isValidMode(struct handle_mode_data data, int start, int& total_cnt, int& need_cnt) {
  std::string search = "oiktl";
  char mode_char;
  bool is_plus = true;
  if (data.mode_data[start][0] != '+' && data.mode_data[start][0] != '-')
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE",
                                   data.client);  // sendCmdResponceはfalse(0)返す
  if (data.mode_data[start][0] == '-') is_plus = false;
  for (size_t i = 1; i < data.mode_data[start].size(); i++) {
    mode_char = data.mode_data[start][i];
    if (search.find(mode_char) != std::string::npos)
      total_cnt++;
    else if (mode_char == '+')
      is_plus = true;
    else if (mode_char == '-')
      is_plus = false;
    else
      return Server::sendCmdResponce(ERR_NOCHANMODES, data.mode_data[0],
                                     data.client);  // mode_data[0] == channelname
    if (data.mode_data[start][i] == 'o' || data.mode_data[start][i] == 'k' ||
        (is_plus == true && data.mode_data[start][i] == 'l'))
      need_cnt++;
  }
  if (total_cnt < 1 || total_cnt > 3)
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
  return true;
}

bool Channel::toggleOperatorPrivileges(struct handle_mode_data& data) {
  std::string target_nick;
  int is_ope = false;
  ClientData* target_client;

  data.param_i++;
  target_nick = data.mode_data[data.param_i];
  if ((target_client = getMemberByNickname(target_nick)) == NULL &&
      (target_client = getOperatorByNickname(target_nick)) == NULL)
    return Server::sendCmdResponce(ERR_USERNOTINCHANNEL, target_nick, "MODE", data.client);
  if (data.is_active == true && target_nick == data.client.getNickname())
    return Server::sendCmdResponce(ERR_NOPRIVILEGES, data.client);
  if (isOperator(target_client) == true) is_ope = true;
  if (data.is_active == true && is_ope == false) {
    removeMember(target_client);
    operators_.push_back(target_client);
  } else if (data.is_active == false && is_ope == true) {
    removeOperator(target_client);
    addMember(target_client);
  }
  return true;
}

void Channel::toggleInviteOnlyChannel(struct handle_mode_data data) {
  if (data.is_active == true) {
    setInviteOnly(true);
  } else {
    setInviteOnly(false);
  }
}

bool Channel::toggleChannelKey(struct handle_mode_data& data) {
  std::string msg;
  data.param_i++;
  if (data.is_active == true) {
    if (!getKey().empty())
      return Server::sendCmdResponce(ERR_KEYSET, getChannelname(), data.client);
    if (isValidKey(data.mode_data[data.param_i]) == false)
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
    setKey(data.mode_data[data.param_i]);
  } else {
    if (!getKey().empty()) setKey("");
  }
  return true;
}

bool Channel::isValidKey(const std::string& key) {
  if (key.size() < 1 || key.size() > 23) return false;
  for (size_t i = 0; i < key.size(); i++) {
    if (std::isspace(key[i])) return false;
  }
  return true;
}

void Channel::toggleTopicPrivileges(struct handle_mode_data data) {
  if (data.is_active == true) {
    setTopicRestricted(true);
  } else
    setTopicRestricted(false);
}

bool Channel::toggleChannelLimit(struct handle_mode_data& data) {
  size_t limit = 0;

  if (data.is_active == true) {
    data.param_i++;
    if ((limit = convertStringToUserLimit(data.mode_data[data.param_i])) == 0)
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
    setUserLimit(limit);
  } else {
    if (getUserLimit() > 0) setUserLimit(0);
  }
  return true;
}

size_t Channel::convertStringToUserLimit(const std::string& l_param) {
  size_t limit = 0;
  std::stringstream ss;
  const int INT_MAX_LEN = 10;
  if (l_param.size() > INT_MAX_LEN) return 0;
  ss << l_param;
  ss >> limit;
  if (limit > INT_MAX || limit < 1) return 0;
  return limit;
}
