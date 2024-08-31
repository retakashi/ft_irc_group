#include "echoServer.hpp"

echoServer::echoServer(){}
echoServer::echoServer(short port):port_(port){}
echoServer::~echoServer(){}
echoServer::echoServer(const echoServer &other)
{ *this = other;}
echoServer &echoServer::operator=(const echoServer &other)
{
  if (this != &other)
  {
    port_ = other.port_;
    size_t i = 0;
    for(i = 0;other.buf_[i] != '\0';i++)
      this->buf_[i] = other.buf_[i];
    this->buf_[i] = '\0';
  }
  return (*this);
}

//socket作成、socketに割り当てるアドレスやポート番号をsockaddrに設定。
void echoServer::initSocket(int &sock,struct sockaddr_in &sockaddr)
{
    int on = 1;
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        putError("socket failed");
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        putError("setsockopt failed");
    std::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(port_);
    if (bind(sock, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
      putError("bind failed");
    //SOMAXCONN: 環境によって最大値が異なるが、私の環境だと128
    if (listen(sock, SOMAXCONN) < 0) 
        putError("listen failed");
    //fcntlはここで良いのか分からん acceptNewClient関数のみで良い？？？？？？？？？
    if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
        putError("fcntl failed");
}

void echoServer::initSelectArgs(fd_set &read_fds,fd_set &write_fds,struct timeval &timeout)
{
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
}

void echoServer::setReadfds(int sock, std::vector<int> clients, fd_set &read_fds)
{
    FD_SET(sock, &read_fds);
    for (size_t i = 0;i < clients.size(); i++)
        FD_SET(clients[i], &read_fds);
}

int echoServer::acceptNewClient(int sock,std::vector<int> &clients)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int new_sock = 0;
    if ((new_sock = accept(sock, (struct sockaddr *) &client_addr, &addr_len)) < 0)
        putError("accept failed");
    if (fcntl(new_sock, F_SETFL, O_NONBLOCK) < 0)
        putError("fcntl failed");
    clients.push_back(new_sock);
    return (new_sock);
}

void echoServer::ft_recv(int sock) {
    int recv_size = 0;

    while (true) {
        recv_size = recv(sock, buf_, RCVBUFSIZE, 0);
        if (recv_size < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
            continue;
        else if (recv_size < 0)
            putError("recv failed");
        else
            break;
    }   
    buf_[recv_size - 1] = '\0';
    buf_[recv_size] = '\r';
    buf_[recv_size + 1] = '\n';
    std::cout << "received: " << buf_ << std::endl;
}

void echoServer::putError(const char *errmsg) {
  perror(errmsg);
  throw std::exception();
}

void echoServer::startServer()
{
    int sock;
    struct sockaddr_in sockaddr;
    //listenできるところまでsocketを設定
    initSocket(sock,sockaddr);
    
    //echoServerはrecvだけのため,read_fdsのみ使用.write_fdsはsendの時に使用？
    fd_set read_fds, write_fds;
    struct timeval timeout;
    std::vector<int> clients;
    int sel_ret = 0;
    initSelectArgs(read_fds,write_fds,timeout);
    while(true)
    {
        int read_sock = 0;
        //後々write_fdsも使用できるようにする？ 
        setReadfds(sock, clients,read_fds);
        if ((sel_ret = select(FD_SETSIZE, &read_fds, &write_fds, NULL, &timeout)) < 0)
            putError("select failed");
        std::cout << "select : " << sel_ret << std::endl;
        if (sel_ret == 0)
        {
            std::cout << "Time out" << std::endl;
            break;
        }
        //新しいクライアント登録
        if (FD_ISSET(sock,&read_fds))
            acceptNewClient(sock,clients);
        //selectが実行されると,read_fdsの中身が準備完了になったclients[i]だけ残る -> FD_ISSETで準備完了になったfdを探す.
        for (size_t i = 0; i < clients.size(); ++i)
        {
            if (FD_ISSET(clients[i], &read_fds))
            {
                FD_CLR(clients[i],&read_fds);
                ft_recv(clients[i]);
            }
        }
    }
}
