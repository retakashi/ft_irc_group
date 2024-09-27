#include "CmdResponse.hpp"

std::string createCmdRespMsg(const std::string& servername, int code) {
  std::stringstream ss;
  ss << ":" << servername;
  switch (code) {
    case ERR_NONICKNAMEGIVEN:
      ss << " :No nickname given";
      break;
    case ERR_NOTREGISTERED:
      ss << " :You have not registered";
      break;
    case ERR_ALREADYREGISTRED:
      ss << " :Unauthorized command (already registered)";
      break;
    case ERR_PASSWDMISMATCH:
      ss << " :Password incorrect";
      break;
  }
  return ss.str();
}

std::string createCmdRespMsg(const std::string& servername, int code, const std::string &str) {
  std::stringstream ss;
  ss << ":" << servername << " ";
  switch (code) {
    case ERR_NOSUCHNICK:
      ss << str << " :No such nick/channel";
      break;
    case ERR_NOSUCHSERVER:
      ss << str << " :No such server";
      break;
    case ERR_NOSUCHCHANNEL:
      ss << str << " :No such channel";
      break;
    case ERR_CANNOTSENDTOCHAN:
      ss << str << " :Cannot send to channel";
      break;
    case ERR_TOOMANYCHANNELS:
      ss << str << " :You have joined too many channels";
      break;
    case ERR_UNKNOWNCOMMAND:
      ss << str << " :Unknown command";
      break;
    case ERR_ERRONEUSNICKNAME:
      ss << str << " :Erroneous nickname";
      break;
    case ERR_NICKNAMEINUSE:
      ss << str << " :Nickname is already in use";
      break;
    case ERR_NEEDMOREPARAMS:
      ss << str << " :Not enough parameters";
      break;
  }
  return ss.str();
}

std::string createCmdRespMsg(const std::string& servername, int code, const std::string &str1, const std::string& str2)
{
 std::stringstream ss;
  ss << ":" << servername << " ";
  switch (code) {
    case ERR_USERNOTINCHANNEL:
      ss << str1 << " " << str2 << " ::They aren't on that channel";
      break;
  }
  return ss.str();
}
