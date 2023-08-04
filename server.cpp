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

int checkUserCmd(std::string Args)
{
    int check = 0;
    for (size_t i = 0; i < Args.size(); i++)
    {
        if (Args[i] != ' ')
            check++;
    }
    if (check == 4)
        return 0;
    return 1;
}


void    Server::Authentication()
{
    std::string cmd[3] = {"PASS", "NICK", "USER"};
    for (size_t i = 0; i < tokens.size(); i = i + 2)
    {
        for (int i = 0; i < 3; i++)
        {
            if (!tokens[i].compare(cmd[i]))
                break;
        }
        std::cout << "kkkkkkk" << std::endl;
        switch (i)
        {
        case 0:
            if (!tokens[i + 1].compare(this->PASS))
                this->pass = TRUE;
            break;
        case 1:
            this->nickNames.push_back(tokens[i + 1]);
            break;
        case 2:
            if (!checkUserCmd(tokens[i + 1]))
                break;
        default:
            std::string failure = "\033[1;31mPLEASE TRY AGAIN\033[0m\n";
            tokens.clear();
            send(clientSocket, failure.c_str(),failure.size() + 1, 0);
            break;
        }
        // if (!tokens.empty() && !tokens[i].compare("PASS") && !tokens[i + 1].compare(this->PASS))
        // {
            
        //     this->pass = TRUE;
        //     std::string mssg = "\033[0;32mYOUR GOOD HERMANOS\033[0m\n";
        //     send(clientSocket, mssg.c_str(), mssg.size() + 1, 0);
        // }
        // else
        // {
        //     std::string failure = "\033[1;31mPLEASE TRY AGAIN\033[0m\n";
        //     tokens.clear();
        //     send(clientSocket, failure.c_str(),failure.size() + 1, 0);
        // }
    }
}

/////////////////////////////////////////////////////////////////////////////

void    Server::client_handling()
{
    std::cout << "WELCOME TO OUR IRC" << std::endl;
}

void    Server::ft_server()
{
    struct sockaddr_in address;
    struct sockaddr_in clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
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

    if (listen(this->server_fd, SOMAXCONN) < 0) 
        throw std::runtime_error("Error: listen");

    std::cout << "Server listening on port 4444..." << std::endl;
    struct pollfd fds[1024];

    memset(fds, 0, sizeof(fds));

    fds[0].fd = this->server_fd;
    fds[0].events = POLLIN | POLLOUT;

    client_fd = 0;
    while (true)
    {
        int pollResult = poll(fds, client_fd + 1, -1);
        if (pollResult == -1) {
            perror("Error in poll");
            break;
        }

        if (pollResult == 0) {
            continue;
        }
        if (fds[0].revents && POLLIN)
        {
            this->pass = FALSE;
            tokens.clear();
            this->clientSocket = accept(this->server_fd, (struct sockaddr *)&clientAddr , &addrSize);
            if (clientSocket == -1) {
                perror("Error accepting connection");
                continue;
            }
            std::string mssg = "NEW CONNECTION\n";
            send(this->clientSocket, mssg.c_str(), mssg.size() + 1, 0);
            if (client_fd >= 1024) {
                std::cout << "Maximum number of clients reached. Rejecting new connection." << std::endl;
                close(clientSocket);
            }
            else {
                    std::cout << "New connection established. Client IP: "
                          << inet_ntoa(clientAddr.sin_addr) << ", Client Port: "
                          << ntohs(clientAddr.sin_port) << std::endl;
                    fds[client_fd + 1].fd = clientSocket;
                    fds[client_fd + 1].events = POLLIN;
                    ++client_fd;
                    client_socket.push_back(clientSocket);
                }
        }
        for (int i = 1; i <= client_fd; ++i)
        {
            char buffer[1024];

            if (fds[i].revents && POLLIN)
            {
                this->valread = recv(fds[i].fd, buffer, strlen(buffer), 0);
                std::cout << valread << std::endl;
                if (this->valread == 0)
                {
                    std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;

                    close (fds[i].fd);
                    fds[i] = fds[client_fd];
                    --client_fd;
                    continue;
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
                    if (tokens.size() == 6)
                        Authentication();
                }
            }
        }
    }
    for (int i = 0; i < clientSocket; ++i) {
        close(fds[i + 1].fd);
    }

    close (this->server_fd);
}

