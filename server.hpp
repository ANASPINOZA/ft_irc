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
#include <string>

#define PORT 1500

class Server {
    public:
        void    ft_server();
        void    SomeParss(char **av);
        void    CheckPort(char *port);
        void    get_PASS(char *pass);
    private:
        std::vector<int>    tab;
        std::vector<int>    client_socket;
        std::string PASS;
        int server_fd;
        // int server;
        int new_socket;
        int valread;
        int Port;
        fd_set master;
};

#endif