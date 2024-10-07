#include "Server.hpp"

int Server::acceptNewClient() {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int new_client_sock = 0;

  if ((new_client_sock = accept(Server::serversock_, (struct sockaddr*)&client_addr, &addr_len)) < 0)
    putFunctionError("accept failed");
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
  std::cout << "connected sockfd: " << new_client_sock << std::endl;
  ClientData new_client(new_client_sock);
  clients_.push_back(new_client);
  return (new_client_sock);
}

void Server::authenticatedNewClient(ClientData& client) {
  std::string command;
  std::string param;
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size <= 0) return;
  std::string casted_msg(msg_, recv_size);
  splitCmdAndParam(casted_msg, command, param);
  if (!client.getAuth() && command != "PASS") {
    sendCmdResponce(ERR_NOTREGISTERED, client);
    return;
  }
  if (command == "PASS")
    handlePass(param, client);
  else if (client.getAuth()) {
    if (command != "NICK" && command != "USER") {
      sendCmdResponce(ERR_NOTREGISTERED, client);
      return;
    }
    if (command == "NICK")
      handleNICK(param, client);
    else if (command == "USER")
      handleUSER(param, client);
  }
  if (client.isCompleteAuthParams() == true) sendWelcomeToIrc(client);
}

void Server::sendWelcomeToIrc(ClientData client) {
  std::stringstream ss;
  ss << ":" << servername_ << " 001 Welcome to the Internet Relay Network " << client.getNickname()
     << "!" << client.getUsername() << "@" << hostname_;
  ft_send(ss.str(), client);
}
