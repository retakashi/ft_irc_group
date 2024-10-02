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
    return nullptr;//c+11
}

void Server::addChannel(const std::string& channelName, Channel* channel) {
    channels_[channelName] = channel;
}

void Server::handleJoin(const std::string& channelName, ClientData& client) {
    if (channelName.empty()) {
        sendCmdResponce(ERR_NEEDMOREPARAMS, "JOIN", client);
        return;
    }
    try {
        Channel* channel = getChannelByName(channelName);
        if (!channel) {
            channel = new Channel(channelName);
            addChannel(channelName, channel);
        }

        if (channel->isMember(&client)) {
            sendCmdResponce(ERR_USERONCHANNEL, channelName, client);
            return;
        }

        channel->addClient(&client);

        // チャンネルにJOINメッセージを送信
        std::string joinMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + " JOIN :" + channelName + "\r\n";
        channel->broadcastMessage(joinMsg, &client);  // Notify all clients in the channel, including the new one
        ft_send(joinMsg, client);  // Notify the new client

        // チャンネルのトピックを確認し、適切なレスポンスコードを送信
        if (channel->getTopic().empty()) {
            std::string noTopicMsg = createCmdRespMsg(servername_, 331, client.getNickname(), channelName + " :No topic is set");
            ft_send(noTopicMsg, client);
        } else {
            std::string topicMsg = createCmdRespMsg(servername_, 332, client.getNickname(), channelName + " :" + channel->getTopic());
            ft_send(topicMsg, client);
        }

        // メンバーリストを送信
        std::string nameList = ":" + servername_ + " 353 " + client.getNickname() + " = " + channelName + " :" + channel->getMemberList() + "\r\n";
        ft_send(nameList, client);
        std::string endOfNames = ":" + servername_ + " 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list.\r\n";
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
