#ifndef SERVER
#define SERVER

#include <iostream>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include "client.hpp"
#include <sys/poll.h>
#include <sstream>
#include "Channel.hpp"
#include "./commands/Commands.hpp"

#define TRUE true
#define FALSE false

class Client;
class Channel;
class commands;

class Server {
    public:
        Server();
        void    ft_server();
        void    SomeParss(char **av);
        void    CheckPort(char *port);
        void    get_PASS(char *pass);
        // bool    Authentication(int idx);
        bool Authentication(Server &s, int fds_fd, int idx);
        // void    client_handling(Server &server,int idx);
        void    client_handling(Server &server, int fds_fd);
        // bool    isNickThere(std::string nickName);
        bool isNickThere(Server &s, std::string nickName);
        bool    isNickInChannel(Server &server, std::string nickName, std::string channelName);
        int     getFdOfExistedClient(std::string nickName, Server &server);
        bool    isChannelIsThere(std::string channelName);
        // void    parseUserInfos(std::string userInfos, int client_fd);
        void parseUserInfos(Server &s, std::string userInfos, int client_fd);
        // Client  getClient(std::string nickName); //Mountassir
        Client getClient(Server &server, std::string nickName); // Mountassir
        Client getClientFromChannel(Server &server, std::string nickName, std::string channelName);
        // Channel  getChannelByName(std::string channelName);
        Channel &getChannelByName(std::string channelName);
        std::map<std::string, Channel> getChannels();
        std::map<std::string, Channel> channel;
        std::map<int, Client > client;
        // struct pollfd fds[1024];
        std::vector<pollfd> fds;
        bool    checkPASS(Server &s, std::string param, int idx, int fds_fd);
        bool    checkNICK(Server &s, std::string nick, int fd, int idx);
        bool    checkUSER(Server &s, std::string user, int fd, int idx);
        void    resetBool();
        void    Failure(Server &s, int fds_fd, int idx);
        bool    isFdThere(Server &s, int fd);        
    private:
        struct sockaddr_in address;
        struct sockaddr_in clientAddr;
        std::vector<int>    tab;
        std::vector<int>    client_socket;
        std::vector<std::string> tokens;
        std::string PASS;
        int server_fd;
        int client_fd;
        int clientSocket;
        int valread;
        int Port;
        bool    pass;
        bool    nick;
        bool    user;
        bool    Authen;

};

#endif
