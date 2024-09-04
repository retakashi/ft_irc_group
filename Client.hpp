#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

class Client {
public:
    Client(int socket);
    void sendMessage(const std::string& message);
    std::string getName() const;
    int getSocket() const; //add

private:
    int socket_;
    std::string name_;
};

#endif
