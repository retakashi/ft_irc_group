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

int main(int argc, char **argv) {
  if (argc != 3)
		{std::cout << "Usage: " << argv[0] << " <port number> <password>" << std::endl; return 1;}
  try {
    g_sig_flg = false;
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
     sa.sa_handler = sigHandler;
    if (sigaction(SIGINT, &sa, NULL) < 0) putSignalError();
    if (sigaction(SIGQUIT, &sa, NULL) < 0) putSignalError();
    // ここでコマンドライン引数のエラー処理を入れる。
    short port = std::atoi(argv[1]);
    std::string password = argv[2];
    Server serv(port, password);
    serv.startServer();
  } catch (const std::exception &e) {
  }
  return 0;
}
