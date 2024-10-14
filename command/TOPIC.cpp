#include "../Channel.hpp"
#include "../Server.hpp"

/*
TOPIC <channel> [ <topic> ]
<topic> が指定されていない場合は、チャネル <channel> のトピックが返される。
<topic> パラメータが指定された場合、そのチャネルのトピックが変更される。
<topic> パラメータが空文字列の場合、そのチャネルのトピックは削除される。-> "TOPIC #channel :"
*/

int Server::handleTOPIC(std::string param, ClientData& client) {
  Channel* ch;
  std::string ch_name;
  std::string msg;
  if (setAndSearchChannel(param, ch_name, client) == false) return 0;
  ch = channels_[ch_name];

  if (param == ch_name) {
    if (ch->getTopic().empty())
      return sendCmdResponce(RPL_NOTOPIC, ch_name, client);
    else
      return sendCmdResponce(RPL_TOPIC, ch_name, ch->getTopic(), client);
  }
  if (ch->getTopicRestricted() == true && ch->isOperator(&client) == false)
    return sendCmdResponce(ERR_CHANOPRIVSNEEDED, ch_name, client);
  if (isValidTopic(param) == false) return 0;
  if (param.empty())
    ch->setTopic("");
  else
    ch->setTopic(param);
  msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + hostname_ + " TOPIC " + ch_name + " " +
        ch->getTopic();
  ch->sendAll(msg);
  return 0;
}

bool Server::setAndSearchChannel(std::string& param, std::string& ch_name, ClientData client) {
  if (param.empty())
    return Server::sendCmdResponce(ERR_NEEDMOREPARAMS, "MODE", client);  // false返す
  std::string::size_type pos = param.find(' ');
  if (pos == std::string::npos)
    ch_name = param;
  else {
    ch_name = param.substr(0, pos);
    ch_name[pos] = '\0';
    param = param.substr(pos + 1);
  }
  std::map<std::string, Channel*>::iterator it = Server::channels_.find(ch_name);
  if (it == Server::channels_.end())
    return Server::sendCmdResponce(ERR_NOSUCHCHANNEL, ch_name, client);
  return true;
}

// 先頭に:があれば取る
bool Server::isValidTopic(std::string& param) {
  std::string nocrlfcl("\0\r\n", 4);
  std::string nospcrlfcl("\0\r\n ", 5);
  bool has_trailing = false;
  if (param[0] == ':') has_trailing = true;
  for (size_t i = 0; i < param.size(); i++) {
    if (has_trailing == true && nocrlfcl.find(param[i]) != std::string::npos)
      return false;
    else if (has_trailing == false && nospcrlfcl.find(param[i]) != std::string::npos)
      return false;
  }
  if (has_trailing == true) param = param.substr(1);
  return true;
}
