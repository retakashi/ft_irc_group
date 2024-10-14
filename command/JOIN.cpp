#include "Channel.hpp"
#include "Server.hpp"

/*
 JOIN <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] / "0"
  チャンネル名は100文字まで。それ以上はリサイズする。
*/
void Server::handleJoin(const std::string& params, ClientData& client) {
  std::istringstream iss(params);
  std::string ch_name;
  std::string key;
  iss >> ch_name >> key;

  if (ch_name.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, "JOIN", client);
    return;
  }
  if (isValidChannelname(ch_name) == false) {
    sendCmdResponce(ERR_NOSUCHCHANNEL, ch_name, client);
    return;
  }
  try {
    Channel* channel = getChannelByName(ch_name);
    if (channel != NULL &&
        (channel->isMember(&client) == true || channel->isOperator(&client) == true)) {
      // sendCmdResponce(ERR_USERONCHANNEL, channelName, client);
      return;
    }
    if (!channel) {
      channel = new Channel(ch_name);
      addChannel(ch_name, channel);
      channel->addOperator(&client);
      ft_send(channel->createJoinMsg(getHostname(), client), client);
      return;
    }
    if (channel->getInviteOnly() == true && channel->isInvitee(&client) == false) {
      sendCmdResponce(ERR_INVITEONLYCHAN, channel->getChannelname(), client);
      return;
    }
    if (!channel->getKey().empty() && channel->getKey() != key) {
      std::string errorMsg =
          createCmdRespMsg(servername_, client.getNickname(), ERR_BADCHANNELKEY, ch_name);
      ft_send(errorMsg, client);
      return;
    }
    if (channel->getUserLimit() > 0 && channel->CountMembers() >= channel->getUserLimit()) {
      Server::sendCmdResponce(ERR_CHANNELISFULL, channel->getChannelname(), client);
      return;
    }
    // 既存のチャンネルに参加
    channel->addMember(&client);
    if (channel->isInvitee(&client) == true) channel->removeInvitee(&client);
    channel->sendAll(channel->createJoinMsg(getHostname(), client));
  } catch (const std::bad_alloc& e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    closeAllSocket();
    throw std::exception();
  }
}

bool Server::isValidChannelname(std::string& channelName) {
  std::string except("\0\a\r\n ,:", 7);
  if (channelName.size() > 100) channelName.resize(100);
  if (channelName[0] != '#' || channelName == "#") return false;
  for (size_t i = 1; i < channelName.size(); i++) {
    if (except.find(channelName[i]) != std::string::npos) return false;
  }
  return true;
}

void Server::addChannel(const std::string& channelName, Channel* channel) {
  channels_.insert(std::make_pair(channelName, channel));
}

std::string Channel::getMembersList() const {
  std::string members_list = ":";
  for (size_t i = 0; i < operators_.size(); i++) {
    members_list += "@";
    members_list += operators_[i]->getNickname();
    if (i != operators_.size() - 1) members_list += " ";
  }
  for (size_t i = 0; i < members_.size(); i++) {
    if (i == 0) members_list += " ";
    members_list += members_[i]->getNickname();
    if (i != members_.size() - 1) members_list += " ";
  }
  return members_list;
}

std::string Channel::createJoinMsg(const std::string& hostname, const ClientData& client) {
  std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + hostname +
                        " JOIN :" + getChannelname() + "\r\n";
  if (!getTopic().empty())
    joinMsg += createCmdRespMsg(Server::servername_, client.getNickname(), RPL_TOPIC,
                                getChannelname(), getTopic()) +
               "\r\n";
  else
    joinMsg +=
        createCmdRespMsg(Server::servername_, client.getNickname(), RPL_NOTOPIC, getChannelname()) +
        "\r\n";
  joinMsg += createCmdRespMsg(Server::servername_, client.getNickname(), RPL_NAMREPLY,
                              getChannelname(), getMembersList()) +
             "\r\n";
  joinMsg +=
      createCmdRespMsg(Server::servername_, client.getNickname(), RPL_ENDOFNAMES, getChannelname());
  return joinMsg;
}
