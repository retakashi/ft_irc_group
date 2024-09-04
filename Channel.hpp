#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

class Client;

class Channel {
public:
    Channel();
    Channel(const std::string& name);
    void addClient(Client* client);
    void removeClient(Client* client);
    void broadcastMessage(const std::string& message, Client* sender);
    void kickClient(Client* client, Client* target);//add↓
    void inviteClient(Client* client, Client* target);
    void setTopic(Client* client, const std::string& topic);
    void setMode(Client* client, char mode, bool enable);
    bool isOperator(Client* client) const;

private:
    std::string name;
    std::string topic;
    std::vector<Client*> clients_;
    std::set<Client*> operators_;
    bool inviteOnly;//add↓
    bool topicRestricted;
    std::string password;
    size_t userLimit;
};

#endif // CHANNEL_HPP
