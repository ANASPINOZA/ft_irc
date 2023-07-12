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
        int     CheckSocket();
        void    SomeParss(char **av);
        void    CheckPort(char *port);
    private:
        std::vector<int>    tab;
        int server_fd;
        int server;
        int new_socket;
        // int valread;
        int Port;
};

#endif