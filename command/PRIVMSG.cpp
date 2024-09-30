#include "../Server.hpp"

void Server::handlePrivateMessage(const std::string param, ClientData &client) 
{
    // チャンネルへの送信か個人への送信かの場合わけをする
    // handle_channel_pmsg()
    // handle_personel_pmsg()
    // これ以降に,区切りで複数の対象に送れれるようにする

    std::string targets, message;
    size_t spacePos = param.find(' ');

    if (spacePos != std::string::npos) 
    {
        targets = param.substr(0, spacePos);

        size_t colonPos = param.find(':', spacePos);
        if (colonPos != std::string::npos) 
            message = param.substr(colonPos + 1);
        else 
        {
            sendCmdResponce(ERR_NOTEXTTOSEND, client);
            return;
        }
    }
    else 
    {
        sendCmdResponce(ERR_NOTEXTTOSEND, client);
        return;
    }
    std::cout << "message : " << message << std::endl;
    
    if (targets[0] == '#')
        handle_privmsg_channel(targets, message, client);
    else 
        handle_privmsg_personal(targets, message, client);

    return ;
}


void  Server::handle_privmsg_channel(std::string targets, std::string message, ClientData &client)
{
    // カンマ区切りで何のこのチャンネルに送るのかを確認する処理

    // チャンネルを探す処理

    // チャンネルを見つけたらそこの中にいる人全員に送る処理

    // whileかforで繰り返す。

    // 個人のやつができたからそれをベースにチャンネル送信もできるようにする。
    return ;
}


void  Server::handle_privmsg_personal(std::string targets, std::string message, ClientData &client)
{
    // ここにtargetを格納したリストを使って保持する。
     // while文で見つけたら、
    std::istringstream iss(targets);
    std::string target;

    while (getline(iss, target, ','))
    {
        ClientData* recipient = getClientByNickname(target);
        if (target.empty() || !recipient || (recipient == &client)) 
        {
            // send_error_message rtakashiに確認 ERR_NORECIPIENT
            sendCmdResponce(ERR_NORECIPIENT, client);
            return;
        }

        // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
        std::string recipientMessage = ":" + (&client)->getNickname() + "!" + (&client)->getUsername() + "@" + "localhost" + " PRIVMSG " + target + " :" + message + "\r\n";
        // std::strncpy(msg_, recipientMessage.c_str(), MAX_BUFSIZE - 1);
        // msg_[MAX_BUFSIZE - 1] = '\0';
        ft_send(recipientMessage, *recipient);
    }
    return ;
}