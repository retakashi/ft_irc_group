#include <algorithm>
#include <sstream>

#include "../Channel.hpp"
#include "../ClientData.hpp"
#include "../Server.hpp"

// JOIN <channel> *( "," <channel> ) [ <key> *( "," <key> ) ] / "0"

class Server;

Channel* Server::getChannelByName(const std::string& channelName) {
  std::map<std::string, Channel*>::iterator it = channels_.find(channelName);
  if (it != channels_.end()) {
    return it->second;
  }
  return NULL;
}

void Server::addChannel(const std::string& channelName, Channel* channel) {
  channels_[channelName] = channel;
}

void Server::handleJoin(const std::string& params, ClientData& client) {
  std::istringstream iss(params);
  std::string channelName;
  std::string key;
  iss >> channelName >> key;

  if (channelName.empty()) {
    sendCmdResponce(ERR_NEEDMOREPARAMS, client.getNickname(), "JOIN", client);
    return;
  }
  try {
    Channel* channel = getChannelByName(channelName);
    if (channel != NULL && (channel->isMember(&client)== true || channel->isOperator(&client) == true))
    {
      sendCmdResponce(ERR_USERONCHANNEL, channelName, client);
      return;
    }
    if (!channel) {
      channel = new Channel(channelName);
      addChannel(channelName, channel);
      channel->addOperator(&client);
    }
    if (!channel->getKey().empty() && channel->getKey() != key) {
      std::string errorMsg =
          createCmdRespMsg(servername_, client.getNickname(), ERR_BADCHANNELKEY, channelName);
      ft_send(errorMsg, client);
      return;
    }
    if (channel->getUserLimit() > 0 && channel->CountMember() >= channel->getUserLimit()) {
      Server::sendCmdResponce(ERR_CHANNELISFULL, channel->getChannelname(), client);
      return;
    }
    if (channel->isOperator(&client) == false) channel->addMember(&client);
    std::cout << "list: " << channel->createJoinMsg(getHostname(), client) << std::endl; 
    // チャンネルにJOINメッセージを送信
    ft_send(channel->createJoinMsg(getHostname(), client), client);
    std::string msg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1" +
                        " JOIN " + channel->getChannelname()+"\r\n";
    channel->broadcastMessage(msg, &client);
    // あとで確認↓
  } catch (const std::bad_alloc& e) {
    std::cerr << "Exception in handleJoin: " << e.what() << std::endl;
    std::string errorMsg =
        createCmdRespMsg(servername_, client.getNickname(), 451, ":Error processing JOIN command");
    ft_send(errorMsg, client);
    throw std::exception();
  }
}

std::string Channel::getMemberList() const {
  std::string member_list = ":";
  for (size_t i = 0; i < operators_.size(); i++) {
    member_list += "@";
    member_list += operators_[i]->getNickname();
    if (i != operators_.size() - 1) member_list += " ";
  }
  for (size_t i = 0; i < member_.size(); i++) {
    if (i == 0) member_list += " ";
    member_list += member_[i]->getNickname();
    if (i != member_.size() - 1) member_list += " ";
  }
  return member_list;
}

std::string Channel::createJoinMsg(const std::string& hostname, const ClientData& client) {
  std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + "127.0.0.1" +
                        " JOIN " + getChannelname() + "\r\n";
  joinMsg += createCmdRespMsg(Server::servername_, client.getNickname(), RPL_NAMREPLY,
                              getChannelname(), getMemberList()) +
             "\r\n";
  joinMsg +=
      createCmdRespMsg(Server::servername_, client.getNickname(), RPL_ENDOFNAMES, getChannelname()) + "\r\n";
  if (!getTopic().empty())
    joinMsg += createCmdRespMsg(Server::servername_, client.getNickname(), RPL_TOPIC,
                                getChannelname(), getTopic()) + "\r\n";
  return joinMsg;
}
