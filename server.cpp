#include "server.hpp"
#include <string>


Server::Server()
{
    this->pass = FALSE;
}

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

void    Server::get_PASS(char *pass)
{
    this->PASS = pass;
}

void    Server::SomeParss(char **av)
{
    CheckPort(av[1]);
    get_PASS(av[2]);
}


void    Server::Authentication()
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


    for (int i = 0; i < FD_SETSIZE; i++)  
    {
        this->client_socket.insert(client_socket.begin() + i, 0);
    }

    if (listen(this->server_fd, 3) < 0) 
        throw std::runtime_error("Error: listen");

    std::cout << "Waiting for connection...." << std::endl;



    while (true)
    {
        FD_ZERO(&this->master);

        FD_SET(this->server_fd, &this->master);

        int max_sd = this->server_fd;

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            this->client_fd = this->client_socket[i];

            if (this->client_fd > 0)
                FD_SET(this->client_fd, &this->master);
            if (this->client_fd > max_sd)
                max_sd = this->client_fd;
        }

        int activity = select(max_sd + 1, &this->master, nullptr, nullptr, nullptr);

        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error\n");  
        }

        if (FD_ISSET(this->server_fd, &this->master))
        {
            this->new_socket = accept(this->server_fd, NULL, NULL);

            // send mssg to client to inform that mssg is sent
            std::string mssg = "NEW CONNECTION\n";
            send(this->new_socket, mssg.c_str(), mssg.size() + 1, 0);

            for (int i = 0; i < FD_SETSIZE; i++)  
            {  
                //if position is empty 
                if( this->client_socket[i] == 0)  
                {  
                    this->client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                         
                    break;  
                }  
            }  
        }
        for (int i = 0; i < max_sd; i++)
        {
            char *buffer = new char;
            this->client_fd = this->client_socket[i];

            if (FD_ISSET(this->client_fd, &this->master))
            {
                this->valread = recv(this->client_fd, buffer, 3000, 0);
                if (this->valread == 0)
                {
                    std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;

                    close (this->client_fd);
                    this->client_socket[i] = 0;
                }
                else {
                    std::string input = buffer;
                    std::string delimiter = " ";

                    size_t pos = 0;
                    std::string token;
                    if ((pos = input.find(delimiter)) != std::string::npos) {
                        token = input.substr(0, pos);
                        tokens.push_back(token);
                        input.erase(0, pos + delimiter.length());
                        tokens.push_back(input.substr(0, input.find("\n")));
                    }
                }
            }
        }
    }
}

