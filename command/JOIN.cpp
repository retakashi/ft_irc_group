#include "Server.hpp"
#include "ClientData.hpp"
#include "Channel.hpp"
#include <sstream>

void handleJoinCommand(Server& server, ClientData& client, const std::string& params) {
    std::istringstream iss(params);
    std::string channelName;
    iss >> channelName;

    Channel* channel = server.getChannelByName(channelName);
    if (!channel) {
        channel = new Channel(channelName);
        server.addChannel(channelName, channel);
    }

    channel->addClient(&client);

    // クライアントにJOINメッセージを送信
    std::string joinMsg = createCmdRespMsg(server.getServerName(), "JOIN " + channelName);
    std::string sendMsg = createSendMsg(joinMsg);
    ft_send(client, sendMsg);

    // チャンネルのトピックを確認し、適切なレスポンスコードを送信
    if (channel->getTopic().empty()) {
        sendCmdResponse(server, client, 331); // 331はトピックが設定されていない場合のコード
    } else {
        //チャンネルにトピックが設定されている場合は、コード332を使用してトピックを送信
        std::string topicMsg = createCmdRespMsg(server.getServerName(), "332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic());
        std::string sendTopicMsg = createSendMsg(topicMsg);
        ft_send(client, sendTopicMsg);
    }
}

void handleKickCommand(Server& server, ClientData& client, const std::string& params) {
    std::istringstream iss(params);
    std::string channelName, targetNick, reason;
    iss >> channelName >> targetNick;
    std::getline(iss, reason);
    if (!reason.empty() && reason[0] == ' ') {
        reason = reason.substr(1);
    }

    Channel* channel = server.getChannelByName(channelName);
    if (channel) {
        ClientData* target = server.getClientByNickname(targetNick);
        if (target) {
            channel->kickClient(&client, target, reason);
            sendCmdResponse(server, client, 441); // 441はユーザーがチャンネルにいない場合のコード
        } else {
            sendCmdResponse(server, client, 401); // 401はユーザーが存在しない場合のコード
        }
    } else {
        sendCmdResponse(server, client, 403); // 403はチャンネルが存在しない場合のコード
    }
}

void handleInviteCommand(Server& server, ClientData& client, const std::string& params) {
    std::istringstream iss(params);
    std::string channelName, targetNick;
    iss >> channelName >> targetNick;

    Channel* channel = server.getChannelByName(channelName);
    if (channel) {
        ClientData* target = server.getClientByNickname(targetNick);
        if (target) {
            channel->inviteClient(&client, target);
            sendCmdResponse(server, client, 341); // 341は招待が成功した場合のコード
        } else {
            sendCmdResponse(server, client, 401); // 401はユーザーが存在しない場合のコード
        }
    } else {
        sendCmdResponse(server, client, 403); // 403はチャンネルが存在しない場合のコード
    }
}

void handleTopicCommand(Server& server, ClientData& client, const std::string& params) {
    std::istringstream iss(params);
    std::string channelName, topic;
    iss >> channelName;
    std::getline(iss, topic);
    if (!topic.empty() && topic[0] == ' ') {
        topic = topic.substr(1);
    }

    Channel* channel = server.getChannelByName(channelName);
    if (channel) {
        channel->setTopic(&client, topic);
        sendCmdResponse(server, client, 332); // 332はトピックが設定された場合のコード
    } else {
        sendCmdResponse(server, client, 403); // 403はチャンネルが存在しない場合のコード
    }
}