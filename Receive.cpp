#include "Server.hpp"

// std::string Server::ft_recv(int socket) {
//   ssize_t recv_size = 0;
//   std::string result_msg;
//   // while (true) {
//     recv_size = recv(socket, msg_, MAX_BUFSIZE, 0);
//     std::cout << "recv ret: " << recv_size << std::endl;
//     if (recv_size < 0) {
//       perror("recv failed");
//       return "";
//     }
//     if (recv_size == 0) {
//       std::cout << "client send EOF(recv)..." << std::endl;
//       Server::disconnectClient(socket);
//       return "";
//     }
//     if (msg_[recv_size - 2] == '\r' && msg_[recv_size - 1] == '\n') {
//       msg_[recv_size - 2] = '\0';
//       result_msg += msg_;
//       std::cout << "recv CRLF: " << recv_size << " " << result_msg << std::endl;
//       return result_msg;
//     }
//     // if (recv_size > MAX_BUFSIZE - 2) recv_size = MAX_BUFSIZE - 2;
//     // if (msg_[recv_size - 1] == '\n') {
//     //   msg_[recv_size - 1] = '\0';
//     //   result_msg += msg_;
//     //   std::cout << "recv : " << recv_size << " " << result_msg << std::endl;
//       return result_msg;
//     // }
//     // msg_[recv_size] = '\0';
//     // result_msg += msg_;
//   // }
// }

std::string Server::ft_recv(int socket) {
  ssize_t recv_size = 0;
  std::string result_msg;

  while (true) {
    memset(msg_, 0, MAX_BUFSIZE);
    recv_size = recv(socket, msg_, MAX_BUFSIZE, 0);
    std::cout << "recv ret: " << recv_size << std::endl;
    if (recv_size < 0) {
      perror("recv failed");
      return "";
    }
    if (recv_size == 0) {
      std::cout << "client send EOF(recv)..." << std::endl;
      Server::disconnectClient(socket);
      return "";
    }
    result_msg += std::string(msg_, recv_size);
    if (result_msg.size() >= 2 && result_msg.substr(result_msg.size() - 2) == "\r\n") {
      result_msg.erase(result_msg.size() - 2);
      std::cout << "recv CRLF: " << recv_size << " " << result_msg << std::endl;
      return result_msg;
    }
  }
}
