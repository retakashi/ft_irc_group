#include "../Server.hpp"

void Server::handlePrivateMessage(const std::string param, ClientData &client) 
{
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
            std::string errorMsg = ":" + servername_ + " 412 " + client.getNickname() + ": No text to send";
            ft_send(errorMsg, client);
            return;
        }
    }
    else 
    {
        std::string errorMsg = ":" + servername_ + " 412 " + client.getNickname() + ": No text to send";
        ft_send(errorMsg, client);
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
    std::string errorMsg = ":";

    while (getline(iss, target, '.'))
    {
        Channel* channel = getChannelByName(target);
        if (target[0] != '#') 
            errorMsg = ":" + servername_ + " 401 " + channel->getChannelname() + " :No such nick/channel";
        else if (!(channel->isMember(&client)))
            errorMsg = ":" + servername_ + " 404 " + channel->getChannelname() + " :Cannot send to channel";
        else if (!channel) 
            errorMsg = ":" + servername_ + " 411 " + channel->getChannelname() + " :No recipient given (PRIVMSG)";
        if (errorMsg != ":")
        {
            ft_send(errorMsg, client);
            return;
        }

        // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
        std::string message_ch = ":PRIVMSG " + target + " :" + message + "\r\n";
        (channel)->broadcastMessage(message_ch, &client);
    }
    return ;
}


void  Server::handle_privmsg_personal(std::string targets, std::string message, ClientData &client)
{
    std::istringstream iss(targets);
    std::string target;
    std::string errorMsg = ":";

    while (getline(iss, target, '.'))
    {
        ClientData* recipient = getClientByNickname(target);
        if (recipient == &client)
            errorMsg = ":" + servername_ + " 411 " + client.getNickname() + " :No recipient given (PRIVMSG)\r\n";
        else if (!recipient) 
            errorMsg = ":" + servername_ + " 401 " + client.getNickname() + " :No such nick/channel";
        if (errorMsg != ":")
        {
            ft_send(errorMsg, client);
            return;
        }

        // 対象者へメッセージを送信する。messageについて RFC(1459, 2.3.1 BNF)
        std::string recipientMessage = ":PRIVMSG " + target + " :" + message + "\r\n";
        ft_send(recipientMessage, *recipient);
    }
    return ;
}
