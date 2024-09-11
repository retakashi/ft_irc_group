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
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ClientData.hpp"
class ClientData;
// ircのメッセージの長さは、最大で512文字（CR-LFを含む）
// （つまり、コマンドとそのパラメータに許される最大文字数は510文字。）文字列の後に"\r\n"がつく
// ->それ以上はぶった斬る
#define MAX_BUFSIZE 510
#define SUCCESS 0
#define FAILURE -1
// メンバー変数は最後に_を付けてます
static bool g_sig_flg;
class Server {
 private:
  short port_;
  int socket_;
  // クライアントのsocket 後々消して、Clientクラスに入れる？
  std::vector<ClientData> clients_;
  char msg_[MAX_BUFSIZE];
  Server();
  void initServerSocket(struct sockaddr_in &sockaddr);
  void initSelectArgs(fd_set &read_fds, int &max_sock, struct timeval &timeout);
  void setReadfds(fd_set &read_fds);
  int getMaxSocket();
  int acceptNewClient();
  int authenticatedNewClient(int client_sock);
  bool isValidNickname(std::string& param);
  void ft_recv(int socket);
  void disconnectClient(ClientData client);
  std::string::size_type splitCommand(std::string casted_msg, std::string &command);
  void splitParam(std::string casted_msg, std::string &param, std::string::size_type pos);
  void ft_send(ClientData client, size_t send_size);
  size_t createSendMsg(const std::string& casted_msg);
  int printWelcomeToIrc(const ClientData &client);
  int printCmdResponce(int code, const std::string &str);
  int printCmdResponce(int code, const ClientData& client, const std::string &str);
 public:
  Server(short port);
  ~Server();
  Server(const Server &other);
  Server &operator=(const Server &other);
  void startServer();
};
void putFunctionError(const char *errmsg);
#endif
