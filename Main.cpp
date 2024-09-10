#include "Server.hpp"

// QUITコマンドできたら消す
void sigHandler(int sig) {
  g_sig_flg = true;
  std::cout << "signal received" << std::endl;
  throw std::exception();
}

void putFunctionError(const char *errmsg) {
  perror(errmsg);
  throw std::exception();
}

int main() {
  try {
    g_sig_flg = false;
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
    // QUITコマンドできたら消して、SIG_IGNの方をコメント解除する
     sa.sa_handler = sigHandler;
    //  sa.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &sa, NULL) < 0) putFunctionError("sigaction failed");
    if (sigaction(SIGQUIT, &sa, NULL) < 0) putFunctionError("sigaction faild");
    // ここでコマンドライン引数のエラー処理を入れる。
    short port = 4242;
    Server serv(port);
    serv.startServer();
  } catch (const std::exception &e) {
  }
  return 0;
}
