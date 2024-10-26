#include "Channel.hpp"
#include "Server.hpp"
#include "CmdResponse.hpp"

// LIST <channel> *( "," <channel> ) [ <target> ]

void Server::handleList(const std::string& params, ClientData& client) {
    std::istringstream iss(params);
    std::string channelName;
    iss >> channelName;

    if (channelName.empty()) {
        for (std::map<std::string, Channel*>::iterator it = channels_.begin(); it != channels_.end(); ++it) {
            Channel* channel = it->second;

            // チャンネルメンバー数を文字列に変換
            std::stringstream ss;
            ss << channel->CountMembers();  // チャンネルのメンバー数をストリームに出力

            // チャンネル情報の作成
            std::string channelInfo = ":server 322 " + client.getNickname() + " " + channel->getChannelname() + " " + ss.str() + " :" + channel->getTopic();
            ft_send(channelInfo, client);
        }
    } else {
        Channel* channel = getChannelByName(channelName);
        if (channel) {
            std::stringstream ss;
            ss << channel->CountMembers();  // チャンネルのメンバー数をストリームに出力

            std::string channelInfo = ":server 322 " + client.getNickname() + " " + channel->getChannelname() + " " + ss.str() + " :" + channel->getTopic();
            ft_send(channelInfo, client);
        } else {
            std::string errorMsg = createCmdRespMsg(servername_, client.getNickname(), ERR_NOSUCHCHANNEL, channelName + " :No such channel");
            ft_send(errorMsg, client);
        }
    }

    // リストの終わりメッセージを送信
    std::string listEnd = ":" + servername_ + " 323 " + client.getNickname() + " :End of /LIST";
    ft_send(listEnd, client);
}
