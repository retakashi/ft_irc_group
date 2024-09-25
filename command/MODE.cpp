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
  Channel channel;
  std::vector<std::string> mode_data;

  if (param.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  // if (isOperator(client) == false)
  // {
  // sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelname, client);
  // return ;
  // }
  /*
  paramからchannelname切り出す->map.find(channelname)で探す。なければmap.end()が戻る
  */
  std::map<std::string, ClientData*>::iterator it = searchChannel(param, client);
  if (it == channel_.end()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  setModeData(param, mode_data, client);
  if (mode_data.size() == 0) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  // bool is_active = false;
  // for (size_t i = 0; i < mode_data.size(); i++) {
  //   for (size_t j = 0; mode_data[i][j] != '\0'; j++) {
  //     if (mode_data[i][0] != '+' && mode_data[i][0] != '-') {
  //       i++;
  //       continue;
  //     }
  //     switch (mode_data[i][j]) {
  //       case '+':
  //         is_active = true;
  //         break;
  //       case '-':
  //         is_active = false;
  //         break;
  //       case 'o':
  //         toggleOperatorPrivileges(mode_params, is_active);
  //         break;
  //       case 'i':
  //         toggleInviteOnlyChannel(mode_params, is_active);
  //         break;
  //       case 'k':
  //         toggleChannelKey(mode_params, is_active);
  //         break;
  //       case 't':
  //         toggleTopicPrivileges(mode_params, is_active);
  //         break;
  //       case 'l':
  //         toggleChannelLimit(mode_params, is_active);
  //         break;
  //       default:
  //         sendCmdResponce();  // 無効なmode
  //     }
  //   }
  // }
}

// Channelクラス内のチャンネル名に必ず接頭辞がついていると信じて
std::map<std::string, ClientData*>::iterator Server::searchChannel(std::string& param,
                                                               ClientData client) {
  std::string channelname;
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) return channel_.end();
  channelname = param.substr(0, pos);
  channelname[pos] = '\0';
  std::map<std::string, ClientData*>::iterator it = channel_.find(channelname);
  if (it != channel_.end()) param = param.substr(pos + 1);
  return it;
}

void Server::setModeData(std::string& param, std::vector<std::string>& mode_data,
                         ClientData client) {
  std::string mode;
  const int limit = 3;
  int cnt = 0;
  if (param.empty() || (param[0] != '+' && param[0] != '-')) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) {
    mode_data.push_back(param);
    return;
  }
  while (param.size() > 0 && cnt < limit) {
    pos = param.find(' ');
    if (pos == std::string::npos) {
      mode_data.push_back(param);
      break;
    }
    mode = param.substr(0, pos);
    mode[pos] = '\0';
    if ((mode[0] == '+' || mode[0] == '-') && isValidMode(mode) == false) {
      sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
      mode_data.clear();
      return;
    }
    mode_data.push_back(mode);
    param = param.substr(pos + 1);
  }
}

bool Server::isValidMode(const std::string& mode) {
  int cnt = 0;
  for (size_t i = 1; mode[i] != '\0'; i++) {
    if (mode[i] == 'o' || mode[i] == 'i' || mode[i] == 'k' || mode[i] == 't' || mode[i] == 'l')
      cnt++;
    else
      return false;
  }
  if (cnt <= 0 || cnt > 2) return false;
  return true;
}
