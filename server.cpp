#include "server.hpp"
#include <string>

void    Server::CheckPort(char *port)
{
    for (size_t i = 0; i < strlen(port); i++)
    {
        if (!isdigit(port[i]))
            throw std::runtime_error("PORT: Error");
    }
    this->Port = atoi(port);
    if (this->Port < 1000 || this->Port > 65535)
        throw std::runtime_error("PORT: Error");
}

void    Server::SomeParss(char **av)
{
    CheckPort(av[1]);
}

int Server::CheckSocket()
{
    for (size_t i = 0; i < tab.size(); i++)
    {
        if (tab[i] == server)
            return (0);
    }
    return (1);
}

void    Server::ft_server()
{
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

  
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Error: socket failed");
  
    // Forcefully attaching socket to the port 8080
    if (setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
        throw std::runtime_error("Error: setsockopt");
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->Port);
    std::cout << "PORT: {" << this->Port << "}" << std::endl;
    // Forcefully attaching socket to the port 8080
    if (bind(this->server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("Error: bind failed");

    std::cout << "Waiting for connection...." << std::endl;

    if (listen(this->server_fd, 3) < 0) 
        throw std::runtime_error("Error: listen");

    fd_set master;



    int max_sd = this->server_fd;
    int max_clients = 30;
    int sd;
    int client_socket[30];
    while (true)
    {
        FD_ZERO(&master);
        FD_SET(this->server_fd, &master);

        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &master);
            if (sd > max_sd)
                max_sd = sd;
        }

        int socketcount = select(max_sd + 1, &master, nullptr, nullptr, nullptr);
        if ((socketcount < 0) && (errno!=EINTR))  
        {  
            printf("select error");  
        }

        if (FD_ISSET(this->server_fd, &master))
        {
            this->new_socket = accept(this->server_fd, NULL, NULL);

            // send mssg to client to inform that mssg is sent
            std::string mssg = "HHHHH I GOT YOU";
            send(this->new_socket, mssg.c_str(), mssg.size() + 1, 0);

            for (int i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }
        
        for (int i = 0; i < max_clients; i++)
        {
            char *buffer = new char;
            sd = client_socket[i];

            if (FD_ISSET(sd, &master))
            {
                int valRead = recv(sd, buffer, 3000);
                if (valRead == 0)
                {
                    std::
                }
            }
        }

    }
    // while (1)
    // {
    //     char *buffer = new char;
    //     if (CheckSocket())
    //     {
    //         if (((this->server = accept(this->server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0))
    //             throw std::runtime_error("Error: accept");
    //     }
    //     this->valread = read(this->server, buffer, 30000);
    //     std::cout << buffer;
    //     delete buffer;
    //     bzero(buffer, strlen(buffer));
    //     std::cout << buffer;
    //     tab.push_back(this->server);
    // }
}
