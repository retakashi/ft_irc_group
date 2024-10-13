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

int check_port(char *port)
{
  if (strlen(port) > 5)
    return (1);
  for (int i = 0; i < strlen(port); i++)
    if (!isdigit(port[i]))
      return (1);
  int port_num = std::atoi(port);
  if (port_num < 1024 || port_num > 65535)
    return (1);
  return (0);
}

int main(int argc, char **argv) {
  if (argc != 3)
        {std::cout << "Usage: " << argv[0] << " <port number> <password>" << std::endl; return 1;}
  if (check_port(argv[1]))
    {std::cout << "Usage: " << argv[1] << " <port> must be in 1024 ~ 65535." << std::endl; return 1;}
  try 
  {
    g_sig_flg = false;
    struct sigaction sa;
    sa.sa_flags = SA_RESTART;
     sa.sa_handler = sigHandler;
    if (sigaction(SIGINT, &sa, NULL) < 0) putSignalError();
    if (sigaction(SIGQUIT, &sa, NULL) < 0) putSignalError();

    short port = std::atoi(argv[1]);
    std::string password = argv[2];
    Server serv(port, password);
    serv.startServer();
  } 
  catch (const std::exception &e) 
  {std::cerr << "Exception: " << e.what() << std::endl;}
  return 0;
}
