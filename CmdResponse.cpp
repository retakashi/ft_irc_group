#include "CmdResponse.hpp"
#include "Server.hpp"

std::string createCmdRespMsg(const std::string& servername, const std::string& nick, int code) {
  std::string pre;
  std::string msg;
  pre = ":" + servername;
  switch (code) {
    case ERR_NONICKNAMEGIVEN:
      msg = pre + " 431 " + nick + ": No nickname given";
      break;
    case ERR_ALREADYREGISTRED:
      msg = pre + " 462 " + nick + " :Unauthorized command (already registered)";
      break;
    case ERR_PASSWDMISMATCH:
      msg = pre + " 464 " + nick + " :Password incorrect";
      break;
  }
  return msg;
}

std::string createCmdRespMsg(const std::string& servername, const std::string& nick, int code,
                             const std::string& str) {
  std::string pre;
  std::string msg;
  pre = ":";
  switch (code) {
    case RPL_CHANNELMODEIS:
      msg = pre + " 324 " + nick + " " + str;
      break;
    case RPL_NOTOPIC:
      msg = pre + " 331 " + nick + " " + str + " :No topic is set";
      break;
    case RPL_ENDOFNAMES:
      msg = pre + " 366 " + nick + " " + str + " :END of /NAMES list";
      break;
    case ERR_NOSUCHNICK:
      msg = pre + " 401 " + nick + " " + str + " :No such nick/channel";
      break;
    case ERR_NOSUCHSERVER:
      msg = pre + " 402 " + nick + " " + str + " :No such server";
      break;
    case ERR_NOSUCHCHANNEL:
      msg = pre + " 403 " + nick + " " + str + " :No such channel";
      break;
    case ERR_CANNOTSENDTOCHAN:
      msg = pre + " 404 " + nick + " " + str + " :Cannot send to channel";
      break;
    case ERR_TOOMANYCHANNELS:
      msg = pre + " 405 " + nick + " " + str + " :You have joined too many channels";
      break;
    case ERR_UNKNOWNCOMMAND:
      msg = pre + " 421 " + nick + " " + str + " :Unknown command";
      break;
    case ERR_ERRONEUSNICKNAME:
      msg = pre + " 432 " + nick + " " + str + " :Erroneous nickname";
      break;
    case ERR_NICKNAMEINUSE:
      msg = pre + " 433 " + nick + " " + str + " :Nickname is already in use";
      break;
    case ERR_NOTONCHANNEL:
      msg = pre + " 442 " + nick + " " + str + " :You're not on that channel";
      break;
    case ERR_USERONCHANNEL:
      msg = pre + " 443 " + nick + " " + str + " :is already on channel";
      break;
    case ERR_NOTREGISTERED:
      msg = pre + " 451 " + nick + " " + str + " :You have not registered";
      break;
    case ERR_NEEDMOREPARAMS:
      msg = pre + " 461 " + nick + " " + str + " :Not enough parameters";
      break;
    case ERR_KEYSET:
      msg = pre + " 467 " + nick + " " + str + " :Channel key already set";
      break;
    case ERR_CHANNELISFULL:
      msg = pre + " 471 " + nick + " " + str + " :Cannot join channel (+l)";
      break;
    case ERR_BADCHANNELKEY:
      msg = pre + " 475 " + nick + " " + str + " :Cannot join channel (+k)";
      break;
    case ERR_NOCHANMODES:
      msg = pre + " 477 " + nick + " " + str + " :Channel doesn't support modes";
      break;
    case ERR_CHANOPRIVSNEEDED:
      msg = pre + " 482 " + nick + " " + str + " :You're not channel operator";
      break;
  }
  return msg;
}

std::string createCmdRespMsg(const std::string& servername, const std::string& nick, int code,
                             const std::string& str1, const std::string& str2) {
  std::string pre;
  std::string msg;
  pre = ":";
  switch (code) {
    case RPL_TOPIC:
      msg = pre + " 332 " + nick + " " + str1 + " " + str2;
      break;
    case RPL_NAMREPLY:
      msg = pre + " 353 " + nick + " @ " + str1 + " " + str2;
      break;
    case ERR_USERNOTINCHANNEL:
      msg = pre + " 441 " + nick + " " + str1 + " " + str2 + " :They aren't on that channel";
      break;
  }
  return msg;
}
