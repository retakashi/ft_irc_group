#include "CmdResponse.hpp"

std::string createCmdRespMsg(const std::string& servername, int code) {
  std::stringstream ss;
  ss << ":" << servername << " ";
  switch (code) {
    case ERR_NONICKNAMEGIVEN:
      ss << ERR_NONICKNAMEGIVEN << " :No nickname given";
      break;
    case ERR_NOTREGISTERED:
      ss << ERR_NOTREGISTERED << " :You have not registered";
      break;
    case ERR_ALREADYREGISTRED:
      ss << ERR_ALREADYREGISTRED << " :Unauthorized command (already registered)";
      break;
    case ERR_PASSWDMISMATCH:
      ss << ERR_PASSWDMISMATCH << " :Password incorrect";
      break;
  }
  return ss.str();
}

std::string createCmdRespMsg(const std::string& servername, int code, const std::string& str) {
  std::stringstream ss;
  ss << ":" << servername << " ";
  switch (code) {
    case RPL_CHANNELMODEIS:
      ss << RPL_CHANNELMODEIS << " " << str;
      break;
    case RPL_NOTOPIC:
      ss << RPL_NOTOPIC << " " << str << " :No topic is set";
      break;
    case ERR_NOSUCHNICK:
      ss << ERR_NOSUCHNICK << " " << str << " :No such nick/channel";
      break;
    case ERR_NOSUCHSERVER:
      ss << ERR_NOSUCHSERVER << " " << str << " :No such server";
      break;
    case ERR_NOSUCHCHANNEL:
      ss << ERR_NOSUCHCHANNEL << " " << str << " :No such channel";
      break;
    case ERR_CANNOTSENDTOCHAN:
      ss << ERR_CANNOTSENDTOCHAN << " " << str << " :Cannot send to channel";
      break;
    case ERR_TOOMANYCHANNELS:
      ss << ERR_TOOMANYCHANNELS << " " << str << " :You have joined too many channels";
      break;
    case ERR_UNKNOWNCOMMAND:
      ss << ERR_UNKNOWNCOMMAND << " " << str << " :Unknown command";
      break;
    case ERR_ERRONEUSNICKNAME:
      ss << ERR_ERRONEUSNICKNAME << " " << str << " :Erroneous nickname";
      break;
    case ERR_NICKNAMEINUSE:
      ss << ERR_NICKNAMEINUSE << " " << str << " :Nickname is already in use";
      break;
    case ERR_NEEDMOREPARAMS:
      ss << ERR_NEEDMOREPARAMS << " " << str << " :Not enough parameters";
      break;
    case ERR_KEYSET:
      ss << ERR_KEYSET  << " " << str << " :Channel key already set";
      break;
    case ERR_CHANNELISFULL:
      ss << ERR_CHANNELISFULL << " " << str << " :Cannot join channel (+l)";
      break;
    case ERR_NOCHANMODES:
      ss << ERR_NOCHANMODES << " " << str << " :Channel doesn't support modes";
      break;
    case ERR_CHANOPRIVSNEEDED:
      ss << ERR_CHANOPRIVSNEEDED << " " << str << " :You're not channel operator";
      break;
  }
  return ss.str();
}

std::string createCmdRespMsg(const std::string& servername, int code, const std::string& str1,
                             const std::string& str2) {
  std::stringstream ss;
  ss << ":" << servername << " ";
  switch (code) {
    case ERR_USERNOTINCHANNEL:
      ss << ERR_USERNOTINCHANNEL << " " << str1 << " " << str2 << " :They aren't on that channel";
      break;
    case RPL_TOPIC:
      ss << RPL_TOPIC << " " << str1 << " " << str2;
      break;
  }
  return ss.str();
}
