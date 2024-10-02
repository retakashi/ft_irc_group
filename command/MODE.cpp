#include "../Channel.hpp"
#include "../Server.hpp"

/*
MODE <channel> <option> <param>
+/-o: param(nick)にオペレーション権限を与える/奪う
+/-i: channelを招待のみに制限/解除.modeparam不要
+/-k: channelにJOINする際にkeyを必要にする/解除
+/-t: TOPICコマンドの権限をオペレータのみ/解除.modeparam不要
+/-l: ユーザー制限(チャンネルに参加できる人数)/解除
*/
int Server::handleMODE(std::string param, ClientData client) {
  handle_mode_data data(client);
  size_t start = 1;
  Channel* ch;

  if (setAndSearchChannel(param, data) == false) return 0;
  // if (channels_[data.mode_data[0]]->isOperator(&client)== false) //要確認
  // return Server::sendCmdResponce(ERR_CHANOPRIVSNEEDED, data.mode_data[0],data.client);
  splitModeParam(param, data.mode_data);
  if (isValidModeData(data) == false) return 0;
  ch = channels_[data.mode_data[0]];

  data.param_i = start;
  while (start < data.mode_data.size()) {
    for (size_t i = 0; data.mode_data[start][i] != '\0'; i++) {
      switch (data.mode_data[start][i]) {
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
  return 0;
}

// paramからchannelnameを切り、mode,mode's paramのみにする
bool Server::setAndSearchChannel(std::string& param, struct handle_mode_data& data) {
  std::string ch_name;
  if (param.empty())
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);  // false返す
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos)
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
  ch_name = param.substr(0, pos);
  ch_name[pos] = '\0';
  std::map<std::string, Channel*>::iterator it = Server::channels_.find(ch_name);
  if (it == Server::channels_.end())
    return Server::sendCmdResponce(ERR_NOSUCHCHANNEL, ch_name, data.client);
  param = param.substr(pos + 1);
  data.mode_data.push_back(ch_name);
  return true;
}

// paramをsplitしてmode_dataに格納
void Server::splitModeParam(std::string& param, std::vector<std::string>& mode_data) {
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
  if (data.mode_data[start][0] != '+' && data.mode_data[start][0] != '-')
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);  // false返す
  for (size_t i = 1; i < data.mode_data[start].size(); i++) {
    if (data.mode_data[start].find(data.mode_data[start][i]) != std::string::npos)
      total_cnt++;
    else
      return Server::sendCmdResponce(ERR_NOCHANMODES, data.mode_data[0],
                                     data.client);  // mode_data[0] == channelname
    if (data.mode_data[start][i] == 'o' ||
        (data.mode_data[start][0] == '+' &&
         (data.mode_data[start][i] == 'k' || data.mode_data[start][i] == 'l')))
      need_cnt++;
  }
  if (total_cnt < 1 || total_cnt > 3)
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
  return true;
}

bool Channel::toggleOperatorPrivileges(struct handle_mode_data& data) {
  std::stringstream ss;
  std::string target_nick;
  int is_ope = false;
  ClientData* target_client;

  data.param_i++;
  target_nick = data.mode_data[data.param_i];
  // target_nickがメンバーかどうか
  if ((target_client = getMemberByNickname(target_nick)) == NULL)
    return Server::sendCmdResponce(ERR_USERNOTINCHANNEL, target_nick, "MODE", data.client);
  // target_nickが既にオペレーターかどうか
  if (isOperator(target_client) == true) is_ope = true;
  if (data.is_active == true && is_ope == false) {
    if (target_nick == data.client.getNickname())  // target's nickname == clientのnickname
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
    operators_.push_back(target_client);
    ss << getChannelname() << " +o by " << data.client.getNickname();  // target_nickのclientに送る
  } else if (data.is_active == false && is_ope == true) {
    ss << getChannelname() << " -o by " << data.client.getNickname();
    // あとでoperator erase作る
    std::cout << "size: " << operators_.size() << std::endl;
    if (operators_.size() > 0) {
      for (size_t i = 0; i < operators_.size(); i++) {
        if (operators_[i] == target_client) operators_.erase(operators_.begin() + i);
      }
    }
  }
  Server::sendCmdResponce(RPL_CHANNELMODEIS, ss.str(), *target_client);
  return true;
}

void Channel::toggleInviteOnlyChannel(struct handle_mode_data data) {
  std::stringstream ss;
  if (data.is_active == true) {
    setInviteOnly(true);
    ss << getChannelname() << " +i by " << data.client.getNickname();
  } else {
    setInviteOnly(false);
    ss << getChannelname() << " -i by " << data.client.getNickname();
  }
  sendOtherMember(ss.str(), data.client);
}

bool Channel::toggleChannelKey(struct handle_mode_data& data) {
  std::stringstream ss;
  if (data.is_active == true) {
    data.param_i++;
    if (!getKey().empty())
      return Server::sendCmdResponce(ERR_KEYSET, getChannelname(), data.client);
    if (isValidKey(data.mode_data[data.param_i]) == false)
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
    setKey(data.mode_data[data.param_i]);
    ss << getChannelname() << " +k " << getKey() << " by " << data.client.getNickname();
  } else {
    if (!getKey().empty()) setKey("");
    ss << getChannelname() << " -k " << " by " << data.client.getNickname();
  }
  sendOtherMember(ss.str(), data.client);
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
  std::stringstream ss;
  if (data.is_active == true) {
    setTopicRestricted(true);
    ss << getChannelname() << " +t by " << data.client.getNickname();
  } else {
    setTopicRestricted(false);
    ss << getChannelname() << " -t by " << data.client.getNickname();
  }
  sendOtherMember(ss.str(), data.client);
}

bool Channel::toggleChannelLimit(struct handle_mode_data& data) {
  size_t limit = 0;
  std::stringstream ss;

  if (data.is_active == true) {
    data.param_i++;
    if ((limit = convertStringToUserLimit(data.mode_data[data.param_i])) == 0)
      return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", data.client);
    ss << getChannelname() << " +l " << limit << " by " << data.client.getNickname();
  } else {
    if (getUserLimit() > 0) {
      setUserLimit(0);
      ss << getChannelname() << " -l by " << data.client.getNickname();
    }
  }
  sendOtherMember(ss.str(), data.client);
  return true;
}

// limit -> とりあえずINT_MAXまで
#define INT_MAX_LEN 10
size_t Channel::convertStringToUserLimit(const std::string& l_param) {
  size_t limit = 0;
  std::stringstream ss;
  if (l_param.size() > INT_MAX_LEN) return 0;
  ss << l_param;
  ss >> limit;
  if (limit > INT_MAX || limit < 1) return 0;
  return limit;
}

void Channel::sendOtherMember(const std::string& str, ClientData me) {
  for (size_t i = 1; i < member_.size(); i++) {
    if (member_[i]->getNickname() != me.getNickname())
      Server::sendCmdResponce(RPL_CHANNELMODEIS, str, *member_[i]);
  }
}
