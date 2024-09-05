#include "Client_data.hpp"

ClientData::Client_data(){}
ClientData::~Client_data(){}
void ClientData::setNickname(std::string& nickname){ nickname_ = nickname;}
void ClientData::setUsername(std::string& username){ username_ = username;}
std::string& ClientData::getNickname(){ return nickname_; } const
std::string& ClientData::getUsername(){ return username_; } const
