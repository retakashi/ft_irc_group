#include "../Server.hpp"
#include "../ClientData.hpp"
#include "../Channel.hpp"
#include <sstream>
#include <algorithm>

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
    try {
        Channel* channel = getChannelByName(channelName);
        if (!channel) {
            channel = new Channel(channelName);
            addChannel(channelName, channel);
        }

        channel->addClient(&client);

        // クライアントにJOINメッセージを送信
        std::string joinMsg = ":" + servername_ + " JOIN " + channelName;
        ft_send(joinMsg, client);

        // チャンネルのトピックを確認し、適切なレスポンスコードを送信
        if (channel->getTopic().empty()) {
            std::string noTopicMsg = createCmdRespMsg(servername_, 331, client.getNickname(), channelName + " :No topic is set");
            ft_send(noTopicMsg, client);
        } else {
            std::string topicMsg = createCmdRespMsg(servername_, 332, client.getNickname(), channelName + " :" + channel->getTopic());
            ft_send(topicMsg, client);
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception in handleJoin: " << e.what() << std::endl;
        std::string errorMsg = createCmdRespMsg(servername_, 451, client.getNickname(), ":Error processing JOIN command");
        ft_send(errorMsg, client);
    }
}
