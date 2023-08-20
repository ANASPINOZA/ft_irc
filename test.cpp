
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


int main()
{
    socklen_t addrSize = sizeof(struct sockaddr_in);
    struct sockaddr_in address;
    struct pollfd   fds[1024];
    struct sockaddr_in clientAddr;

    int server_fd;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Error: socket failed");
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
        throw std::runtime_error("Error: setsockopt");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(4444);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Error: bind failed");
    
    if (listen(server_fd, SOMAXCONN) < 0)
        throw std::runtime_error("Error: listen");
    
    memset(fds, 0, sizeof(fds));
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    int client_fd = 0;
    while (true)
    {
        int pollResult = poll(fds, client_fd + 1, -1);
        if (pollResult == -1)
            std::runtime_error("Error: poll failed");

        if (pollResult == 0)
            continue;
        if (fds[0].revents && POLLIN)
        {
            int clientSocket = accept(server_fd, (struct sockaddr *)&clientAddr, &addrSize);
            if (clientSocket == -1)
            {
                perror("accept error");
                continue;
            }
            else
                {
                    std::cout << "New connection established. Client IP: "
                    << inet_ntoa(clientAddr.sin_addr) << ", Client Port: "
                    << ntohs(clientAddr.sin_port) << std::endl;
                    fds[client_fd + 1].fd = clientSocket;
                    fds[client_fd + 1].events = POLLIN;
                    ++client_fd;
                }
            
        }
        char buffer[1024];
        for (int i = 1; i <= client_fd; ++i)
        {
            memset(buffer, 0, sizeof(buffer) - 1);
            if (fds[i].revents && POLLIN)
            {
                int valread = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                if (valread == 0)
                {
                    std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;
                    close(fds[i].fd);
                    std::cout << "i  : " << i << "client_fd " << client_fd << std::endl;
                    fds[i] = fds[client_fd];
                    --client_fd;
                    continue;
                }
                else
                {
                    std::cout << "HERE" << std::endl;
                    std::cout << buffer;
                }
            }
        }
    }
}