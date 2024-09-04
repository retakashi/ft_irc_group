#include "Client.hpp"
#include <iostream>

Client::Client(int socket) : socket_(socket) {
    // クライアント名を設定するロジックを追加
    name_ = "client_" + std::to_string(socket);
}

void Client::sendMessage(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}

std::string Client::getName() const {
    // クライアント名を返すロジックを実装
    // ここでは仮の実装を示します
    return name_;
}

int Client::getSocket() const {
    return socket_;
}
