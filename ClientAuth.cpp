#include "Server.hpp"

int Server::acceptNewClient() {
  struct sockaddr_in client_addr;
  std::memset(&client_addr, 0, sizeof(client_addr));
  socklen_t addr_len = sizeof(client_addr);
  int new_client_sock = 0;

  if ((new_client_sock = accept(Server::serversock_, (struct sockaddr*)&client_addr, &addr_len)) <
      0)
    putFunctionError("accept failed");
  if (fcntl(new_client_sock, F_SETFL, O_NONBLOCK) < 0) putFunctionError("fcntl failed");
  std::cout << "connected sockfd: " << new_client_sock << std::endl;
  ClientData new_client(new_client_sock);
  clients_.push_back(new_client);
  return (new_client_sock);
}

/*
最初にクライアントから送られてくるメッセージ
CAP LS 302
JOIN :
その後
CAP
CAP END
PASS pass
NICK reira
USER reira reira 127.0.0.1 :Takashima Reira
*/
void Server::authenticatedNewClient(ClientData& client) {
  std::vector<std::string> cmd_with_p;
  std::string cmd;
  std::string param;
  ssize_t recv_size = ft_recv(client.getSocket());
  if (recv_size <= 0) return;
  std::string casted_msg(msg_, recv_size);
  if (casted_msg.find("\r\n") != std::string::npos)
    splitCmds(casted_msg, cmd_with_p);
  else
    cmd_with_p.push_back(casted_msg);
  for (size_t i = 0; i < cmd_with_p.size(); i++) {
    splitCmdAndParam(cmd_with_p[i], cmd, param);
    if (cmd == "CAP") {
      // std::string response = "CAP END";
      // Server::ft_send(response, client);
      std::string response = "CAP * LS :multi-prefix";
      Server::ft_send(response, client);
    } else if (cmd == "PASS")
      handlePass(param, client);
    else if (client.getAuth() == true && cmd == "NICK")
      handleNICK(param, client);
    else if (client.getAuth() == true && cmd == "USER")
      handleUSER(param, client);
    else {
      sendCmdResponce(ERR_NOTREGISTERED,cmd, client);
      return;
    }
  }
  if (client.isCompleteAuthParams() == true)
    sendWelcomeToIrc(client);
}

void Server::sendWelcomeToIrc(ClientData client) {
  std::string welcomemsg = ":" + servername_ + " 001 " + client.getNickname() + " :Welcome to the IRC Network, " + client.getNickname() + "!";
  ft_send(welcomemsg, client);
}
