#include "server.hpp"
#include <vector>

int Server::CheckSocket()
{
    for (size_t i = 0; i < tab.size(); i++)
    {
        if (tab[i] == new_socket)
            return (0);
    }
    return (1);
}

void    Server::ft_server()
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // char *hello = strdup("Hello from server");
  
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Error: socket failed");
  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
        throw std::runtime_error("Error: setsockopt");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(this->server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Error: bind failed");
    if (listen(this->server_fd, 3) < 0) 
        throw std::runtime_error("Error: listen");
    while (1)
    {
        char *buffer = new char;
        if (!CheckSocket())
            std::cout << "Same socketFd" << std::endl;
        else if (((this->new_socket = accept(this->server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0))
            throw std::runtime_error("Error: accept");
        this->valread = read(this->new_socket, buffer, 30000);
        std::cout << buffer;
        delete buffer;
        tab.push_back(this->new_socket);
    }
}
