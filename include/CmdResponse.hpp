#ifndef CMDRESPONSE_HPP
#define CMDRESPONSE_HPP

#include <iostream>
#include <string>
#include <sstream>

enum cmd_resp
{
    RPL_CHANNELMODEIS = 324,
    RPL_NOTOPIC = 331,
    RPL_TOPIC = 332,
    RPL_INVITING = 341,
    RPL_NAMREPLY = 353,
    RPL_ENDOFNAMES = 366,
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_TOOMANYTARGETS = 407,
    ERR_NORECIPIENT = 411,
    ERR_NOTEXTTOSEND = 412,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_TOOMANYCHANNELS = 405,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_UNAVAILRESOURCE = 437,
    ERR_USERNOTINCHANNEL = 441,
    ERR_NOTONCHANNEL = 442,
    ERR_USERONCHANNEL = 443,
    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    ERR_PASSWDMISMATCH = 464,
    ERR_KEYSET = 467,
    ERR_CHANNELISFULL = 471,
    ERR_INVITEONLYCHAN = 473,
    ERR_BADCHANNELKEY = 475,
    ERR_NOCHANMODES = 477,
    ERR_NOPRIVILEGES = 481,
    ERR_CHANOPRIVSNEEDED = 482
};
std::string createCmdRespMsg(const std::string& servername, const std::string& nick, int code);
std::string createCmdRespMsg(const std::string& servername, const std::string& nick, int code, const std::string &str);
std::string createCmdRespMsg(const std::string& servername, const std::string& nick, int code, const std::string &str1, const std::string& str2);
#endif
