#ifndef CLIENTDATA_HPP
#define CLIENTDATA_HPP

#include "echoServer.hpp"

class ClientData
{
    private:
        std::string nickname_;
        std::string username_;
        std::string hostname_;
        std::string servername_;
        sdt::string realname_;

    public:
        ClientData();
        ~ClientData();
        void setNickname(std::string &nickname);
        void setUsername(std::string &username); 
        std::string& getNickname() const;
        std::string& getUsername() const;
};
#endif
