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
#include "client.hpp"
#include <sys/poll.h>

#define PORT 1500
#define TRUE true
#define FALSE false

class Server {
    public:
        Server();
        void    ft_server();
        void    SomeParss(char **av);
        void    CheckPort(char *port);
        void    get_PASS(char *pass);
        void    Authentication();
        void    client_handling();
    private:
        std::vector<int>    tab;
        std::vector<int>    client_socket;
        std::vector<std::string> tokens;
        std::map<int, Client > user;
        std::string PASS;
        int server_fd;
        int client_fd;
        // int server;
        int clientSocket;
        int valread;
        int Port;
        bool    pass;
        // bool    nick;
        // bool    user;
        // fd_set master;
};

#endif
