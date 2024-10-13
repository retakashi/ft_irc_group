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
    std::istringstream iss(targets);
    std::string target;

    while (getline(iss, target, ','))
    {
        Channel* channel = getChannelByName(target);
        if (target[0] != '#' || !channel || !(channel->isMember(&client))) 
        {
            sendCmdResponce(ERR_NORECIPIENT, client);
            return;
        }

        // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
        std::string message_ch = ":" + (&client)->getNickname() + "!" + (&client)->getUsername() + "@" + "localhost" + " PRIVMSG " + target + " :" + message + "\r\n";
        (channel)->broadcastMessage(message_ch, &client);
    }
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
            sendCmdResponce(ERR_NORECIPIENT, client);
            return;
        }

        // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
        std::string recipientMessage = ":" + (&client)->getNickname() + "!" + (&client)->getUsername() + "@" + "localhost" + " PRIVMSG " + target + " :" + message + "\r\n";
        ft_send(recipientMessage, *recipient);
    }
    return ;
}
