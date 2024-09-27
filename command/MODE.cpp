#include "../Server.hpp"

/*
MODE <channel> <option> <param>
+/-o: param(nick)にオペレーション権限を与える/奪う
+/-i: channelを招待のみに制限/解除.modeparam不要
+/-k: channelにJOINする際にkeyを必要にする/解除
+/-t: TOPICコマンドの権限をオペレータのみ/解除.modeparam不要
+/-l: ユーザー制限(チャンネルに参加できる人数)/解除
*/
void Server::handleMODE(std::string param, ClientData& client) {
  std::vector<std::string> mode_data;
  std::string channelname;

  if (param.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  // if (isOperator(client) == false)
  // {
  // sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelname, client);
  // return ;
  // }
  channelname = searchChannel(param, client);
  if (channelname.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  setModeData(param, mode_data, client);
  if (mode_data.size() == 0) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  bool is_active = false;
  size_t i = 0;
  while (mode_data.size() > 0) {
    i = 0;
    while (mode_data[0][i] != '\0') {
      switch (mode_data[0][i]) {
        case '+':
          is_active = true;
          break;
        case '-':
          is_active = false;
          break;
        case 'o':
          if (toggleOperatorPrivileges(mode_data, is_active, channels_[channelname], client) ==
              false)
            return;
          break;
          // case 'i':
          //   toggleInviteOnlyChannel(mode_data, is_active);
          //   break;
          // case 'k':
          //   toggleChannelKey(mode_data, is_active);
          //   break;
          // case 't':
          //   toggleTopicPrivileges(mode_data, is_active);
          //   break;
          // case 'l':
          //   toggleChannelLimit(mode_data, is_active);
          //   break;
          // default:
          //   sendCmdResponce();  // 無効なmode
      }
      i++;
    }
  }
}

// paramからmode,modeoptionを切り出すのも行う
std::string Server::searchChannel(std::string& param, ClientData client) {
  std::string channelname;
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) return "";
  channelname = param.substr(0, pos);
  channelname[pos] = '\0';
  std::map<std::string, Channel>::iterator it = channels_.find(channelname);
  if (it == channels_.end()) return "";
  if (it != channels_.end()) param = param.substr(pos + 1);
  return channelname;
}

void Server::setModeData(std::string& param, std::vector<std::string>& mode_data,
                         ClientData client) {
  std::string mode;

  if (isValidMode(param) == false) return;
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) {
    mode_data.push_back(param);
    return;
  }
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

bool Server::isValidMode(const std::string& mode) {
  int cnt = 0;
  std::string mode_str = "oiktl";
  if (mode[0] != '+' && mode[0] != '-') return false;
  for (size_t i = 1; mode[i] != '\0' && mode[i] != ' '; i++) {
    if (mode_str.find(mode[i]) != std::string::npos)
      cnt++;
    else
      return false;
  }
  if (cnt <= 0 || cnt > 2) return false;
  return true;
}

// 使用したmode's paramがあれば消す
bool Server::toggleOperatorPrivileges(std::vector<std::string> mode_data, bool is_active,
                                      Channel& channel, ClientData client) {
  std::string target_nickname;
  if (mode_data.size() < 2)  //+o param -> size2
    return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
  target_nickname = mode_data[1];
  if (target_nickname == client.getNickname())
    return sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
  std::map<std::string, ClientData*>::iterator member;
  std::map<std::string, ClientData*>::iterator ope;
  member = channel.members_.find(target_nickname);
  if (member == channel.members_.end())
    return sendCmdResponce(ERR_USERNOTINCHANNEL, target_nickname, "MODE", client);
  ope = channel.operators_.find(target_nickname);
  if (is_active == true && ope == channel.operators_.end())
    channel.operators_.insert(std::make_pair(target_nickname, member->second));
  else if (is_active == false && member != channel.operators_.end())
    channel.operators_.erase(member);
  mode_data.erase(mode_data.begin() + 1);
  return true;
}
