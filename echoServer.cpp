#include "echoServer.hpp"

echoServer::echoServer(){}
echoServer::echoServer(short port, const std::string& password): port_(port), server_password_(password) {}
echoServer::~echoServer(){}
echoServer::echoServer(const echoServer &other)
{ *this = other;}

echoServer &echoServer::operator=(const echoServer &other)
{
  if (this != &other)
  {
    port_ = other.port_;
    size_t i = 0;
    for (i = 0; other.msg_[i] != '\0'; i++)
        this->msg_[i] = other.msg_[i];
    this->msg_[i] = '\0';
    this->clients_ = other.clients_;
  }
  return (*this);
}

// socket作成、socketに割り当てるアドレスやポート番号をsockaddrに設定。
void echoServer::initSocket(int &sock, struct sockaddr_in &sockaddr) {
  int on = 1;
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    putError("socket failed");
    //ソケットが閉じられた直後でも、同じポート番号を使用して新しいソケットをバインド出来るようにする
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    putError("setsockopt failed");
  std::memset(&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  sockaddr.sin_port = htons(port_);
  if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    putError("bind failed");
  // SOMAXCONN: 環境によって最大値が異なるが、私の環境だと128
  if (listen(sock, SOMAXCONN) < 0) 
    putError("listen failed");
  // fcntlはここで良いのか分からん.acceptNewClient関数のみで良い？？？？？？？？？
  if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
    putError("fcntl failed");
}

void echoServer::initSelectArgs(fd_set &read_fds, fd_set &write_fds,
                                struct timeval &timeout) {
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  timeout.tv_sec = 100;
  timeout.tv_usec = 0;
}

void echoServer::setReadfds(int sock, fd_set &read_fds) {
  FD_SET(sock, &read_fds);
  for (size_t i = 0;i < clients_.size();i++) 
    FD_SET(clients_[i], &read_fds);
}

int echoServer::acceptNewClient(int sock) {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int new_sock = 0;

  if ((new_sock = accept(sock, (struct sockaddr *)&client_addr, &addr_len)) < 0)
    putError("accept failed");
  if (fcntl(new_sock, F_SETFL, O_NONBLOCK) < 0)
    putError("fcntl failed");
  
  std::cout << "connected sockfd: " << new_sock << std::endl;
  clients_.push_back(new_sock);
  return (new_sock);
}


void echoServer::ft_recv(size_t i) {
    int recv_size = 0;

  while (g_sig_flg == false) {
    recv_size = recv(clients_[i], msg_, RCVBUFSIZE, 0);
    if (recv_size < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
      continue;
    else if (recv_size < 0 && errno == ECONNRESET) 
    {  // nc -c localhost 4242を使用すると、cntrl + D(EOF)が使用できる.
        //クライアント側からEOF, cntrl+C, cntrl+\が送られるとこの条件下に入る.
        disconnectClient(i);
        return ;
    }
    else if (recv_size < 0)
        putError("recv failed");
    else
      break;
  }
  msg_[recv_size - 1] = '\0';
  msg_[recv_size] = '\n';
  msg_[recv_size + 1] = '\r';
  std::cout << "received: " << msg_ << std::endl;
  ft_send(i, recv_size + 1);
}

void echoServer::ft_send(size_t i, size_t send_size) {
  int send_ret = 0;

  while (g_sig_flg == false) {
    send_ret = send(clients_[i], msg_, send_size, 0);
    if (send_ret < 0 && errno == EAGAIN || errno == EWOULDBLOCK)
      continue;
    else if (send_ret < 0 && errno == ECONNRESET)
    {
        disconnectClient(i); 
        return ; 
    }
    else if (send_ret < 0)
      putError("send failed");
    else
      break;
  }
}



// 認証のための改良版
void echoServer::disconnectClient(size_t i) 
{
    std::cout << "Disconnected sockfd : " << clients_[i] << std::endl;
    if (close(clients_[i]) < 0)
        putError("close failed");
    msg_[0] = '\0';
    authenticated_clients_.erase(clients_[i]);
    clients_.erase(clients_.begin() + i);
}
// 認証のための関数（クライアントのメッセージを解読する）
bool echoServer::authenticate(int client_sock) 
{
	char auth_buffer[RCVBUFSIZE];
    int recv_size = recv(client_sock, auth_buffer, RCVBUFSIZE - 1, 0);
    if (recv_size <= 0) 
	{
		// 認証メッセージを受信できなかった
        return false;  
    }
    auth_buffer[recv_size - 1] = '\0';
    std::string message(auth_buffer);
    
	std::istringstream iss(message);
  std::string command, password;

  iss >> command >> password;
  if (iss)
  {
    std::cout << "Too many arguments" << std::endl;
    perror("Too many arguments");
  }
  if (iss.fail())
    perror("Iss failed");

  // std::cout << "ここまでまできた5" << command << ", pass :" << password << std::endl;
	// std::cout << "authenticate まできた6" << std::endl;
	bool answer = false;
	if (password == server_password_)
		answer = true;
	// std::cout << "answer = " << answer << std::endl;
    return (password == server_password_);
}


void echoServer::startServer() 
{
  int sock;
  struct sockaddr_in sockaddr;
  // listenできるところまでsocketを設定
  initSocket(sock, sockaddr);

  // read_fdsのみ使用。write_fds,except_fdsも必要？
  fd_set read_fds, write_fds;
  struct timeval timeout;
  int sel_ret = 0;
  initSelectArgs(read_fds, write_fds, timeout);
  while (g_sig_flg == false) 
  {
    // 後々write_fdsも使用できるようにする？
    setReadfds(sock, read_fds);
    sel_ret = select(FD_SETSIZE, &read_fds, &write_fds, NULL, &timeout);
    if (sel_ret < 0)
      putError("select failed");
    if (sel_ret == 0) 
	{
      std::cout << "Time out" << std::endl;
      break;
    }
    // 新しいクライアント接続
    if (FD_ISSET(sock, &read_fds))
        acceptNewClient(sock);

	for (size_t i = 0; i < clients_.size(); i++) 
	{
		if (FD_ISSET(clients_[i], &read_fds)) 
		{
			std::cout << "ここまでまできた1" << std::endl;
			if (!authenticated_clients_[clients_[i]]) 
			{
				// 未認証クライアントの認証処理
				if (authenticate(clients_[i])) 
				{
					std::cout << "ここまでまできた2" << std::endl;
					std::cout << "Client authenticated: " << clients_[i] << std::endl;
          // ここで認証済みのユーザーのNick ,USERNAMEを入れていきたい。
					authenticated_clients_[clients_[i]] = true;
				} 
				else 
				{
					std::cout << "ここまでまできた3" << std::endl;
					std::cout << "Authentication failed for client: " << clients_[i] << std::endl;
					disconnectClient(i);
					// disconnectClient()でクライアントが削除されるため、インデックスを調整
					i--;
					continue;
				}
			} 
			else 
			{
				// 認証済みクライアントのメッセージ処理
				ft_recv(i);
			}
		}
    }
  }
}

