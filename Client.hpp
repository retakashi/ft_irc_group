// #ifndef CLIENT_HPP
// #define CLIENT_HPP
// #include <iostream>
// #include "echoServer.hpp"

// // class for client
// class Client
// {
//     private:
//         // client file descriptor
//         int sockfd; 
//         // client ip address
//         std::string     IPadd; 
//         bool            is_authenticated();
//         std::string     nickname;
//         std::string     username;
//         std::string     hostname;
//         std::string     mode;
//         // std::string     mode; // ここに他のも追加できる


//     public:
//         Client();
//         ~Client();
//         Client(const Client &other);
//         Client &operator=(const Client &other);



//         // Client(int sockfd) : socket(sock), authenticated(false) {}
//         int             GetFd(){return sockfd;}
//         std::string     GetNickname(){return nickname};
//         std::string     GetUsername(){return username};
//         std::string     GetHostname(){return hostname};
//         // std::string     GetHostname(){return hostname}; //ここに他のも追加していく

//         void            setIpAdd(std::string ipadd){IPadd = ipadd;}
//         void            SetFd(int fd){sockfd = fd;}
//         void            SetNickname(std::string nickname){nickname = nickname;}
//         void            SetUsername(std::string username){username = username;}
//         void            SetNickname(std::string hostname){hostname = hostname;}
//         // void            SetNickname(std::string nick){nickname = nick;}//ここに他のも追加していく


// };

// #endif

/*
クライアントの持ち方としてはこのクラスには一人分に必要なパワフルなクラスを作成する
Serverの方でstd::vector<Clients> clients_のような形で値を持っておいて作業をすると良いのかなと思います。
アクセスはclients_[i]で要素番号にアクセスして中身を確認するような形にする。
*/
