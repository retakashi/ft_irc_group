#include "../Channel.hpp"
#include "../Server.hpp"
#include "../CmdResponse.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>

void Server::handleCommands(ClientData& client) {
  std::string command;
  std::string params;
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size <= 0) return;
  std::string casted_msg(msg_, recv_size);
  splitCmdAndParam(casted_msg, command, params);

  if (command == "NICK")
    handleNICK(params, client);
  else if (command == "PRIVMSG")
    handlePrivateMessage(params, client);
  // else if (command == "OPER")
  //     handleOper(params, client);
  // else if (command == "MODE")
  //   handleMODE(params, client);
  // else if (command == "NOTICE")
  //     handleNotice(params, client);
  else if (command == "JOIN")
      handleJoin(params, client);
  // else if (command == "TOPIC")
  //     setTopic(params, client);
  else if (command == "KICK")
	handleKick(params, client);
//   else if (command == "INVITE")
// 	handleInvite(params, client);
  // else if (command == "PART")
  //     handlePart(params, client);
  // else if (command == "QUIT")
  //     handleQuit(params, client);
  else  // 無効なコマンドが来た時
    sendCmdResponce(ERR_UNKNOWNCOMMAND, command, client);
}

void Server::handleKick(const std::string& params, ClientData& client) { 
    std::istringstream iss(params);
    std::string channelName, targetNickname, reason;
    iss >> channelName >> targetNickname;
    std::getline(iss, reason);
    reason = reason.empty() ? "No reason provided" : reason.substr(1); // Remove leading space

    Channel* channel = getChannelByName(channelName);
    if (channel) {
        ClientData* target = getClientByNickname(targetNickname);
        if (target) {
            channel->kickMember(&client, target, reason);
        } else {
            sendCmdResponce(ERR_NOSUCHNICK, targetNickname, client);
        }
    } else {
        sendCmdResponce(ERR_NOSUCHCHANNEL, channelName, client);
    }
}

void Server::handleInvite(const std::string& params, ClientData& client) {
    // パラメータの解析
    std::istringstream iss(params);
    std::string targetNickname, channelName;
    iss >> targetNickname >> channelName;
    if (targetNickname.empty() || channelName.empty()) {
        sendCmdResponce(ERR_NEEDMOREPARAMS, "INVITE", client);
        return;
    }

    // ターゲットユーザーの存在確認
    ClientData* target = getClientByNickname(targetNickname);
    if (!target) {
        sendCmdResponce(ERR_NOSUCHNICK, targetNickname, client);
        return;
    }

    // チャンネルの存在確認
    Channel* channel = getChannelByName(channelName);
    if (!channel) {
        sendCmdResponce(ERR_NOSUCHCHANNEL, channelName, client);
		return;
    }

    // チャンネルメンバーの確認
    if (!channel->isMember(&client)) {
        sendCmdResponce(ERR_NOTONCHANNEL, channelName, client);
        return;
    }

    // 招待の権限確認
    if (channel->getInviteOnly() && !channel->isOperator(&client)) {
        sendCmdResponce(ERR_CHANOPRIVSNEEDED, channelName, client);
        return;
    }

    // ターゲットユーザーのチャンネル参加確認
    if (channel->isMember(target)) {
        sendCmdResponce(ERR_USERONCHANNEL, targetNickname, client);
        return;
    }

    // 招待の送信
    std::string message = ":" + client.getNickname() + " INVITE " + target->getNickname() + " :" + channelName + "\r\n";
    Server::ft_send(message, *target);
    sendCmdResponce(RPL_INVITING, targetNickname + " " + channelName, client);
}
