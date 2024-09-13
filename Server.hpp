#ifndef Server_HPP
#define Server_HPP
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <climits>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <iostream>

#include "ClientData.hpp"
#include "CmdResponse.hpp"

class ClientData;
/* ircのメッセージの長さは、最大で512文字（CR-LFを含む）
（つまり、コマンドとそのパラメータに許される最大文字数は510文字。）文字列の後に"\r\n"がつく
->それ以上はぶった斬る
*/ 

// メンバー変数は最後に_を付けてます
static bool g_sig_flg;

struct user_data
{
  std::string username;
  char mode;
  std::string realname;
};

class Server {
 private:
  static const int MAX_BUFSIZE = 510;
  short port_;
  int socket_;
  std::string servername_;
  std::string hostname_;
  std::vector<ClientData> clients_;
  char msg_[MAX_BUFSIZE];
  Server();
  void initServerSocket(struct sockaddr_in &sockaddr);
  void initSelectArgs(fd_set &read_fds, int &max_sock, struct timeval &timeout);
  void setReadfds(fd_set &read_fds);
  int getMaxSocket();
  int acceptNewClient();
  void authenticatedNewClient(int client_sock);
  bool isValidNickname(std::string& param, const ClientData& client);
  bool isUserParamValid(const std::string& params, const ClientData& client);
  bool isCompleteAuthParams(const ClientData& client);
  bool isCompleteUserParams(const ClientData& client);
  void ft_recv(int socket);
  void disconnectClient(ClientData client);
  std::string::size_type splitCommand(std::string casted_msg, std::string &command);
  void splitParam(std::string casted_msg, std::string &param, std::string::size_type pos);
  void ft_send(ClientData client, size_t send_size);
  size_t createSendMsg(const std::string& casted_msg);
  void sendWelcomeToIrc(const ClientData &client);
  void sendCmdResponce(int code, const std::string &str, const ClientData &client);
  void sendCmdResponce(int code, const ClientData &client);
  void NICKcmd(std::string casted_msg, std::string::size_type pos, ClientData &client);
  void USERcmd(std::string casted_msg, std::string::size_type pos, ClientData &client)
 public:
  Server(short port);
  ~Server();
  Server(const Server &other);
  Server &operator=(const Server &other);
  void startServer();
};
void putFunctionError(const char *errmsg);
#endif
