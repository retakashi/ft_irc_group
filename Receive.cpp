#include "Server.hpp"

ssize_t Server::ft_recv(int socket) {
  ssize_t recv_size = 0;

  while (1) {
    recv_size = recv(socket, msg_, MAX_BUFSIZE, 0);
    if (recv_size < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
      continue;
    else if (recv_size == 0) {
      std::cout << "client send EOF..." << std::endl;
      disconnectClient(socket);
      return 0;
    } else if (recv_size < 0) {
      putFunctionError("recv failed");
      return -1;
    } else
      break;
  }
  msg_[recv_size - 1] = '\0';
  return recv_size - 1;  // 改行文字分減らす
}
