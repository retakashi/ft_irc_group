#include "Server.hpp"

ssize_t Server::ft_recv(int socket) {
  ssize_t recv_size = 0;
  recv_size = recv(socket, msg_, MAX_BUFSIZE, 0);
  if (recv_size == 0) {
    std::cout << "client send EOF(recv)..." << std::endl;
    Server::disconnectClient(socket);
    return 0;
  }
  if (recv_size < 0) {
    perror("recv failed");
    return -1;
  }
  if (msg_[recv_size - 2] == '\r' && msg_[recv_size - 1] == '\n') {
    msg_[recv_size - 2] = '\0';
    return recv_size - 2;
  }
  msg_[recv_size - 1] = '\0';
  return recv_size - 1;
}
