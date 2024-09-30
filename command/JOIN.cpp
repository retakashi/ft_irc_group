#include "Server.hpp"
#include "ClientData.hpp"
#include "Channel.hpp"
#include <sstream>
#include <algorithm>

// メッセージを生成
std::string createCmdRespMsg(const std::string& servername, const std::string& message) {
    std::ostringstream oss;
    oss << ":" << servername << " " << message;
    return oss.str();
}

// 送信メッセージを生成
std::string createSendMsg(const std::string& resp_msg) {
    return resp_msg + "\r\n";
}

// メッセージを送信
void ft_send(ClientData& client, const std::string& message) {
    send(client.getSocket(), message.c_str(), message.size(), 0);
}

// コマンドのレスポンスを送信
void sendCmdResponse(Server& server, ClientData& client, int code) {
    std::string resp_msg = createCmdRespMsg(server.getServername(), std::to_string(code));
    std::string send_msg = createSendMsg(resp_msg);
    ft_send(client, send_msg);
}

void Server::handleJoin(const std::string& channelName, ClientData& client) {
    Channel* channel = getChannelByName(channelName);
    if (!channel) {
        channel = new Channel(channelName);
        addChannel(channelName, channel);
    }

    channel->addClient(&client);

    // クライアントにJOINメッセージを送信
    std::string joinMsg = createCmdRespMsg(getServername(), "JOIN " + channelName);
    std::string sendMsg = createSendMsg(joinMsg);
    ft_send(client, sendMsg);

    // チャンネルのトピックを確認し、適切なレスポンスコードを送信
    if (channel->getTopic().empty()) {
        sendCmdResponse(*this, client, 331); // 331はトピックが設定されていない場合
    } else {
        std::string topicMsg = createCmdRespMsg(getServername(), "332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic());
        std::string sendTopicMsg = createSendMsg(topicMsg);
        ft_send(client, sendTopicMsg);
    }
}
