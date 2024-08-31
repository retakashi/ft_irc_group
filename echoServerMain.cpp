#include "echoServer.hpp"

int main()
{
    try
    {
        short port = 4242;
        echoServer serv(port);
        serv.startServer();
    }
    catch(const std::exception& e)
    {
    }
    return 0;
}
