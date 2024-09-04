#include "Client.hpp"
#include <iostream>

void Client::sendMessage(const std::string& message) {
    std::cout << "Message: " << message << std::endl;
}