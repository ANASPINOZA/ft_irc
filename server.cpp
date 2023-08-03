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

/////////////////////////////   Authentication /////////////////////////////////

void    Server::Authentication()
{
    char buffer[1024] = "NOOOP";
    if (!tokens.empty() && !tokens[0].compare("PASS") && !tokens[1].compare(this->PASS))
        this->pass = TRUE;
    else
    {
        tokens.clear();
        send(client_fd, buffer, strlen(buffer), 0);
    }
    // if (!user[client_fd].tokens.empty() && !user[client_fd].tokens[0].compare("PASS") && !user[client_fd].tokens[1].compare(this->PASS))
    //     user[client_fd].setPass(true);
}

/////////////////////////////////////////////////////////////////////////////

void    Server::client_handling()
{
    std::cout << "WELCOME TO OUR IRC" << std::endl;
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

    fd_set reads_fd;

    FD_ZERO(&this->master);
    FD_ZERO(&reads_fd);

    while (true)
    {

        FD_SET(this->server_fd, &this->master);

        int max_fd = this->server_fd;

        // for (int i = 0; i < FD_SETSIZE; i++)
        // {
        //     this->client_fd = this->client_socket[i];

        //     if (this->client_fd > 0)
        //         FD_SET(this->client_fd, &this->master);
        //     if (this->client_fd > max_fd)
        //         max_fd = this->client_fd;
        // }

        reads_fd = this->master;
        int activity = select(max_fd + 1, &reads_fd, nullptr, nullptr, nullptr);

        if ((activity < 0) && (errno!=EINTR))  
        {  
            printf("select error\n");  
        }

        for (this->client_fd = 0; this->client_fd <= max_fd; this->client_fd++)
        {
            // std::cout << client_fd << std::endl;
            char *buffer = new char;
            if (FD_ISSET(this->client_fd, &reads_fd))
            {
                // listener scoop
                std::cout << "HHHH" << std::endl;
                if (this->client_fd == this->server_fd)
                {
                    this->new_socket = accept(this->server_fd, NULL, NULL);

                    std::string mssg = "NEW CONNECTION\n";
                    send(this->new_socket, mssg.c_str(), mssg.size() + 1, 0);
                    FD_SET(this->new_socket, &reads_fd);
                    if (this->new_socket > max_fd)
                        max_fd = this->new_socket;
                    printf("selectserver: new connection from %s on socket %d\n", inet_ntoa(address.sin_addr), this->new_socket);
                }
                // handling  Data
                else
                {
                    this->valread = recv(this->client_fd, buffer, 3000, 0);
                    if (this->valread == 0)
                    {
                        std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;
                        close (this->client_fd);
                        FD_CLR(this->client_fd, &master);
                    }
                    else {
                        std::string input = buffer;
                        std::string delimiter = " ";

                        size_t pos = 0;
                        std::string token;
                        if ((pos = input.find(delimiter)) != std::string::npos) {
                            token = input.substr(0, pos);
                            tokens.push_back(token);
                            // user[this->client_fd].addData(token);
                            input.erase(0, pos + delimiter.length());
                            token = input.substr(0, input.find("\n"));
                            tokens.push_back(input.substr(0, input.find("\n")));
                            // user[this->client_fd].addData(token);
                        }
                        Authentication();
                    }

                    // for (int i = 0; i < FD_SETSIZE; i++)  
                    // {  
                    //     //if position is empty 
                    //     if( this->client_socket[i] == 0)  
                    //     {  
                    //         this->client_socket[i] = new_socket;  
                    //         printf("Adding to list of sockets as %d\n" , client_socket[i]);  
                                
                    //         break;  
                    //     }  
                    // } 
                }
            }
        }
        // std::cout << max_fd << std::endl;
        // for (int i = 0; i < max_fd; i++)
        // {
        //     char *buffer = new char;
        //     // std::cout << client_fd << " " << client_socket[i] << std::endl;
        //     // if (client_fd != this->client_socket[i])
        //     // {
        //     //     // std::cout << "fd changes" << std::endl;
        //     //     this->pass = FALSE;
        //     // }
        //     // this->client_fd = this->client_socket[i];

        //     if (FD_ISSET(this->client_fd, &reads_fd))
        //     {
                
        //         std::cout << "HERE" << std::endl;
        //         this->valread = recv(this->client_fd, buffer, 3000, 0);
        //         if (this->valread == 0)
        //         {
        //             std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;
        //             close (this->client_fd);
        //             this->client_socket[i] = 0;
        //         }
        //         else {
        //             std::string input = buffer;
        //             std::string delimiter = " ";

        //             size_t pos = 0;
        //             std::string token;
        //             if ((pos = input.find(delimiter)) != std::string::npos) {
        //                 token = input.substr(0, pos);
        //                 tokens.push_back(token);
        //                 // user[this->client_fd].addData(token);
        //                 input.erase(0, pos + delimiter.length());
        //                 token = input.substr(0, input.find("\n"));
        //                 tokens.push_back(input.substr(0, input.find("\n")));
        //                 // user[this->client_fd].addData(token);
        //             }
        //         }
        //         Authentication();
        //         if (this->pass)
        //         {
        //             // std::cout << client_fd << std::endl;
        //             user[client_fd].addVector(tokens);
        //             // user[client_fd].printData();
        //             // std::cout << user[client_fd].tokens[0] << std::endl;
        //         }
                
        //     }
        // }
    }
}

