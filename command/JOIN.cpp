#include "Server.hpp"
#include "ClientData.hpp"
#include "Channel.hpp"
#include <sstream>
#include <algorithm>

class Server;

// メッセージを生成
std::string createCmdRespMsg(const std::string& servername, const std::string& message) {
    std::ostringstream oss;
    oss << ":" << servername << " " << message;
    return oss.str();
}

// 送信メッセージを生成
size_t Server::createSendMsg(const std::string &casted_msg) {
  std::memset(msg_, 0, sizeof(msg_));
  size_t i = 0;
  while (casted_msg[i] != '\0' && i < MAX_BUFSIZE) {
    msg_[i] = casted_msg[i];
    i++;
  }
  msg_[i] = '\r';
  msg_[i + 1] = '\n';
  return i + 2;
}

// コマンドのレスポンスを送信
void sendCmdResponse(Server& server, ClientData& client, int code) {
    std::string resp_msg = createCmdRespMsg(server.getServername(), std::to_string(code));
    std::string send_msg = createSendMsg(resp_msg);
    Server::ft_send(client, send_msg);
}

void Server::handleJoin(const std::string& channelName, ClientData& client) {
    Channel* channel = getChannelByName(channelName);
    if (!channel) {
        channel = new Channel(channelName);
        addChannel(channelName, channel);
    }

    channel->addClient(&client);

    // クライアントにJOINメッセージを送信
    std::string joinMsg = ":" + getServername() + " JOIN " + channelName + "\r\n";
    ft_send(client, joinMsg.size());

    // チャンネルのトピックを確認し、適切なレスポンスコードを送信
    if (channel->getTopic().empty()) {
        std::string noTopicMsg = ":" + getServername() + " 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n";
        ft_send(client, noTopicMsg.size());
    } else {
        std::string topicMsg = ":" + getServername() + " 332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n";
        ft_send(client, topicMsg.size());
    }
}
