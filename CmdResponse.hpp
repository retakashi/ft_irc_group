#ifndef CMDRESPONSE_HPP
#define CMDRESPONSE_HPP

#include <iostream>
#include <string>
#include <sstream>

enum cmd_resp
{
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER = 402,
    ERR_NOSUCHCHANNEL = 403,
    ERR_CANNOTSENDTOCHAN = 404,
    ERR_TOOMANYCHANNELS = 405,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_UNAVAILRESOURCE = 437,
    ERR_NOTREGISTERED = 451,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    ERR_PASSWDMISMATCH = 464
};
std::string createCmdRespMsg(const std::string& servername, int code);
std::string createCmdRespMsg(const std::string& servername, int code, const std::string &str);
#endif
