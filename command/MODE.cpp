#include "Server.hpp"

/*
MODE <channel> <option> <param>
+/-o: param(nick)にオペレーション権限を与える/奪う
+/-i: channelを招待のみに制限/解除.modeparam不要
+/-k: channelにJOINする際にkeyを必要にする/解除
+/-t: TOPICコマンドの権限をオペレータのみ/解除.modeparam不要
+/-l: ユーザー制限(チャンネルに参加できる人数)/解除
*/
void Server::handleNICK(std::string param, ClientData& client) {
  std::string channel;
  std::string modes;
  std::vector<std::string> mode_params;

  if (param.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  // if (isOperator(client) == false)
  // {
  //     sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelname, client);
  //     return ;
  // }
  channel = setChannelname(param, client);
  if (channel.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  modes = setModes(param, client);
  if (modes.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);
    return;
  }
  setModeParams(param, client, mode_params);
}

// Channelクラス内のチャンネル名に必ず接頭辞がついていると信じて
std::string Server::setChannelname(std::string& param, ClientData client) {
  std::string channel;
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos) return "";
  channel = param.substr(0, pos);
  channel[pos] = '\0';
  if (searchSameChannel(channel) == false) return "";
  param = param.substr(pos + 1);
  return channel;
}

std::string Server::setModes(std::string& param, ClientData client) {
  std::string modes;
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos)
  modes = param;
  else
  {
  modes = param.substr(0, pos);
  modes[pos] = '\0';
  param = param.substr(pos + 1);
  }
  if (isValidModes(modes) == false) return "";
  return modes;
}

void Server::setModeparams(std::string& param, ClientData client, std::vector<std::string>& mode_params)
{
    std::string mode_param;

    std::string::size_type pos;
    while(param.size() > 0)
    {
        pos = param.find(' ');
        if (pos == std::string::npos)
        {
            mode_params.push_back(param);
            break;
        }
        mode_param = param.substr(0,pos);
        mode_param[pos] = '\0';
        mode_params.push_back(mode_param); 
        param = param.substr(pos + 1);
    }
}
