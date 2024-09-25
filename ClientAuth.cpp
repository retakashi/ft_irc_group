#include "Server.hpp"

int Server::acceptNewClient() {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int new_client_sock = 0;

  if ((new_client_sock = accept(socket_, (struct sockaddr *)&client_addr, &addr_len)) < 0)
    putFunctionError("accept failed");
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
  std::cout << "connected sockfd: " << new_client_sock << std::endl;
  ClientData new_client(new_client_sock);
  clients_.push_back(new_client);
  return (new_client_sock);
}

void Server::sendWelcomeToIrc(const ClientData& client) {
  std::stringstream ss;
  size_t send_size = 0;
  ss << ":" << servername_ << " 001 Welcome to the Internet Relay Network " << client.getNickname()
     << "!" << client.getUsername() << "@" << hostname_;
  send_size = createSendMsg(ss.str());
  ft_send(client, send_size);
}
//  これってうまくいったのかな？もしかして
