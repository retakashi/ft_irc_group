#include "echoServer.hpp"

int main()
{
    try
    {
        unsigned short port_arr[2] = {7,555};
        echoServer serv(port_arr);
        serv.startServer(port_arr);
    }
    catch(const std::exception& e)
    {
    }
    return 0;
}
