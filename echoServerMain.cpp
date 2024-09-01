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
/*
bool is_correct_port(char *str)

ポート番号の受け入れに関して関数を作りました。
1. 1-65535の範囲のうち1023まではシステムポート番号
2. 1024-がユーザーポート番号
そのため、エラーは
- 値はstrの長さを5以上
- intに変換した後の値が65535以上、1024以下は受け入れをしない
ようにしてます。

boolにしているけど、必要に応じて値を返す関数に変えても良いと思います。
同時にパスワードの部分も後で変更できるといいのかもしれません。
*/ 

bool is_correct_port(char *str)
{
    if (strlen(str) > 5)
        return (false);
    int num = 0;
    num = std::atoi(str);
    if (num < 1024 || num > 65535)
        return (false);
    return (true);
}

int main()
{
    // char *str_port = "4242";
    short port = 4242;
    try
    {
        struct sigaction sa;
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = sigHandler;
        if (sigaction(SIGINT, &sa, NULL) < 0)
            putError("sigaction failed");
        if (sigaction(SIGQUIT, &sa, NULL) < 0)
            putError("sigaction faild");
        // if (argc != 3 || !(is_correct_port(str_port)))
        // {
        //     throw ("Wrong Arguments. \n");
        // }
        echoServer serv(port);
        serv.startServer();
    }
    catch(const std::exception& e)
    {
    }
    return 0;
}
