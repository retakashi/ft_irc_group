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
#include <map>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "ClientData.hpp"
#include "CmdResponse.hpp"
class ClientData;
/* ircのメッセージの長さは、最大で512文字（CR-LFを含む）
（つまり、コマンドとそのパラメータに許される最大文字数は510文字。）文字列の後に"\r\n"がつく
->それ以上はぶった斬る
*/

// メンバー変数は最後に_を付けてます
static bool g_sig_flg;

struct user_data {
  std::string username;
  char mode;
  std::string unused;
  std::string realname;
};

struct startserv_data {
  struct sockaddr_in sockaddr;
  fd_set read_fds;
  int max_sock;
  struct timeval timeout;
  int sel_ret;
  int client_sock;
};

struct handle_mode_data {
  std::vector<std::string> mode_data;
  ClientData &client;
  bool is_active;
  size_t param_i;
  handle_mode_data(ClientData &c) : client(c), is_active(false), param_i(0) {}
};

class Server {
 private:
  static int serversock_;
  static const int MAX_BUFSIZE = 510;
  short port_;
  std::string serverpass_;
  std::string hostname_;
  char msg_[MAX_BUFSIZE];
  // Server.cpp
  Server();
  void initServerSocket(struct sockaddr_in &sockaddr);
  void setSelectArgs(fd_set &read_fds, int &socket_max);
  const std::string &getServername() const;
  const std::string &getHostname() const;
  // Utils.cppに移行
  void handleClientCommunication(ClientData &client);
  static void putFunctionError(const char *errmsg);
  void splitCmdAndParam(std::string casted_msg, std::string &command, std::string &param);
  // このclientsのgetterは後で別に移動させても良いかもしれません。
  ClientData *getClientByNickname(const std::string &nickname);
  // Receive.cpp
  ssize_t ft_recv(int socket);
  // ClientAuth.cpp USERは認証のみ使用のためこっち
  int acceptNewClient();
  void authenticatedNewClient(ClientData &client);
  void sendWelcomeToIrc(ClientData client);

  // ->Commandディレクトリ
  // Commands.cpp
  void handleCommands(ClientData &client);
  // USER.cpp
  void handleUSER(std::string param, ClientData &client);
  bool isValidUSERparams(std::string &params, struct user_data &user_data,
                         const ClientData &client);
  bool isValidUsername(const std::string &params, std::string &username,
                       std::string::size_type pos);
  bool isValidMiddle(const std::string &params, char &mode, std::string &unused,
                     std::string::size_type pos);
  bool isValidRealname(const std::string &params, std::string &realname);
  // NICK.cpp
  void handleNICK(std::string param, ClientData &client);
  bool isValidNickname(std::string &param, const ClientData &client);
  // PASS.cpp
  void handlePass(std::string param, ClientData &client);
  // PRIVMSG.cpp
  void handlePrivateMessage(const std::string param, ClientData &client);
  void handle_privmsg_channel(std::string targets, std::string message, ClientData &client);
  void handle_privmsg_personal(std::string targets, std::string message, ClientData &client);
  // MODE.cpp
  int handleMODE(std::string param, ClientData client);
  bool setAndSearchChannel(std::string &param, struct handle_mode_data &data);
  void splitModeParam(std::string &param, std::vector<std::string> &mode_data);
  bool isValidModeData(struct handle_mode_data &data);
  bool isValidMode(struct handle_mode_data data, int start, int &total_cnt, int &need_cnt);
  // ここから先は各自で追加していく。
  // void handleJoin(const std::string &params, ClientData &client);
  // void handleKick(const std::string &params, ClientData &client);
  // void handleTopic(const std::string &params, ClientData &client);

 public:
  static std::string servername_;
  static std::vector<ClientData> clients_;
  static std::map<std::string, Channel *> channels_;
  // Server.cpp
  Server(short port, std::string password);
  ~Server();
  void startServer();
  static void disconnectClient(ClientData client);
  static void closeAllSocket();
  // Send.cpp
  static ssize_t ft_send(std::string msg, ClientData client);
  static int sendCmdResponce(int code, ClientData client);
  static int sendCmdResponce(int code, const std::string &str, ClientData client);
  static int sendCmdResponce(int code, const std::string &str1, const std::string &str2,
                             ClientData client);
};
#endif
