#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>

class Client;

class Channel {
public:
    Channel();
    Channel(const std::string& name);
    void addClient(Client* client);
    void removeClient(Client* client);
    void broadcastMessage(const std::string& message, Client* sender);

private:
    std::string name;
    std::string topic;
    std::vector<Client*> clients_;
};

#endif // CHANNEL_HPP
