#include "Server.hpp"

void sigHandler(int sig) {
  g_sig_flg = true;
  std::cout << "signal received" << std::endl;
  throw std::exception();
}

void putSignalError() {
  perror("sigaction failed");
  throw std::exception();
}

int main() {
  try {
    g_sig_flg = false;
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
     sa.sa_handler = sigHandler;
    if (sigaction(SIGINT, &sa, NULL) < 0) putSignalError();
    if (sigaction(SIGQUIT, &sa, NULL) < 0) putSignalError();
    // ここでコマンドライン引数のエラー処理を入れる。
    short port = 4242;
    Server serv(port);
    serv.startServer();
  } catch (const std::exception &e) {
  }
  return 0;
}
