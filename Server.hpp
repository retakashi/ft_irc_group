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
#include <string>
#include <vector>
#include <map>

#include "ClientData.hpp"
#include "CmdResponse.hpp"
#include "Channel.hpp"

class ClientData;
class Channel;
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

class Server {
 private:
  static const int MAX_BUFSIZE = 510;
  short port_;
  std::string serverpass_;
  int socket_;
  std::string servername_;
  std::string hostname_;
  char msg_[MAX_BUFSIZE];
  std::map<std::string, Channel> channels_;
  std::vector<ClientData> clients_;

 public:
  // Server.cpp
  Server();
  void initServerSocket(struct sockaddr_in &sockaddr);
  void setSelectArgs(fd_set &read_fds, int &socket_max);
  int acceptNewClient();
  void handleClientCommunication(ClientData &client);
  ssize_t ft_recv(int socket);
  void disconnectClient(ClientData client);
  void splitCmdAndParam(std::string casted_msg, std::string &command, std::string &param);
  ssize_t ft_send(ClientData client, size_t send_size);
  size_t createSendMsg(const std::string &casted_msg);
  int sendCmdResponce(int code, const std::string &str, const ClientData &client);
  int sendCmdResponce(int code, const ClientData &client);
  const std::string &getServername() const;
  const std::string &getHostname() const;
  void putFunctionError(const char *errmsg);
  void closeAllSocket();
  // ClientAuth.cpp USERは認証のみ使用のためこっち
  void authenticatedNewClient(ClientData &client);
  void handleUSER(std::string param, ClientData &client);
  bool isValidUSERparams(std::string &params, struct user_data &user_data,
                         const ClientData &client);
  bool isValidUsername(const std::string &params, std::string &username,
                       std::string::size_type pos);
  bool isValidMiddle(const std::string &params, char &mode, std::string &unused,
                     std::string::size_type pos);
  bool isValidRealname(const std::string &params, std::string &realname);
  void sendWelcomeToIrc(const ClientData &client);
  // Commandディレクトリ
  void handleCommands(ClientData &client);
  void handleNICK(std::string param, ClientData &client);
  bool isValidNickname(std::string &param, const ClientData &client);
  void handlePass(std::string param, ClientData &client);
  void handleJoin(const std::string &params, ClientData &client);
  void handleKick(const std::string &params, ClientData &client);
  void handleTopic(const std::string &params, ClientData &client);
  void joinCommand(ClientData& client, const std::string& channelName);







  Server(short port, std::string password);
  ~Server();
  Server(const Server &other);
  Server &operator=(const Server &other);
  void startServer();
  // Channel用
  Channel* getChannelByName(const std::string& name);
  void addChannel(const std::string& name, Channel* channel);
  ClientData* getClientByNickname(const std::string& nickname);
  void sendMessage(ClientData& client, const std::string& message);

  

};
#endif