#include "../Server.hpp"
#include "../ClientData.hpp"
#include "../Channel.hpp"
#include <sstream>
#include <algorithm>

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
        sendCmdResponce(ERR_NEEDMOREPARAMS, "JOIN", client);
        return;
    }
    try {
        Channel* channel = getChannelByName(channelName);
        if (!channel) {
            channel = new Channel(channelName);
            addChannel(channelName, channel);
            channel->addOperator(&client);
        }

        if (channel->isMember(&client)) {
            sendCmdResponce(ERR_USERONCHANNEL, channelName, client);
            return;
        }
        if (!channel->getKey().empty() && channel->getKey() != key) {
            std::string errorMsg = createCmdRespMsg(servername_, 475, client.getNickname(), channelName + " :Cannot join channel (+k)");
            ft_send(errorMsg, client);
            return;
        }
        if (channel->getUserLimit() > 0 && channel->CountMember() >= channel->getUserLimit())
        {
            Server::sendCmdResponce(ERR_CHANNELISFULL,channel->getChannelname(),client);
            return ;
        }
        channel->addClient(&client);

        // チャンネルにJOINメッセージを送信
        std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + getHostname() + " JOIN :" + channelName;
        channel->broadcastMessage(joinMsg, &client);  // Notify all clients in the channel, including the new one
        ft_send(joinMsg, client);  // Notify the new client

        // チャンネルのトピックを確認し、適切なレスポンスコードを送信
        if (channel->getTopic().empty()) {
            Server::sendCmdResponce(RPL_NOTOPIC, channel->getChannelname(), client);
        } else {
            sendCmdResponce(RPL_TOPIC, channel->getChannelname(), channel->getTopic(), client);
            // std::string topicMsg = createCmdRespMsg(servername_, 332, client.getNickname(), channelName + " :" + channel->getTopic());
            // ft_send(topicMsg, client);
        }

        // メンバーリストを送信
        std::string nameList = ":" + servername_ + " 353 " + client.getNickname() + " = " + channelName + " :" + channel->getMemberList();
        ft_send(nameList, client);
        std::string endOfNames = ":" + servername_ + " 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.";
        ft_send(endOfNames, client);
    } catch (const std::exception& e) {
        std::cerr << "Exception in handleJoin: " << e.what() << std::endl;
        std::string errorMsg = createCmdRespMsg(servername_, 451, client.getNickname(), ":Error processing JOIN command");
        ft_send(errorMsg, client);
    }
}
std::string Channel::getMemberList() const {
    std::ostringstream oss;
    for (std::vector<ClientData*>::const_iterator it = member_.begin(); it != member_.end(); ++it) {
        if (it != member_.begin()) {
            oss << " ";
        }
        oss << (*it)->getNickname();
    }
    return oss.str();
}
