#include "echoServer.hpp"

//サーバー側でcntrl+C, cntrl+\が送信された時
void sigHandler(int sig)
{
        g_sig_flg = true;
        std::cout << "signal received" << std::endl;
        throw std::exception();
}

void putError(const char *errmsg) {
  perror(errmsg);
  throw std::exception();
}

int main()
{
    try
    {
        struct sigaction sa;
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = sigHandler;
        if (sigaction(SIGINT, &sa, NULL) < 0)
            putError("sigaction failed");
        if (sigaction(SIGQUIT, &sa, NULL) < 0)
            putError("sigaction faild");
        short port = 4242;
        echoServer serv(port);
        serv.startServer();
    }
    catch(const std::exception& e)
    {
    }
    return 0;
}
