#include "server.hpp"

Server::Server()
{
    this->pass = FALSE;
}

void Server::CheckPort(char *port)
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

void Server::get_PASS(char *pass)
{
    this->PASS = pass;
}

void Server::SomeParss(char **av)
{
    CheckPort(av[1]);
    get_PASS(av[2]);
}

std::map<std::string, Channel> Server::getChannels()
{
    return (this->channel);
}

/////////////////////////////   Authentication /////////////////////////////////

int checkUserCmd(std::string Args)
{
    int check = 0;
    for (size_t i = 0; i < Args.size(); i++)
    {
        if (Args[i] == ' ')
            check++;
    }
    if (check == 3)
        return 0;
    return 1;
}

void trimCRLF(std::vector<std::string> &lines)
{
    for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        std::string &line = *it;
        size_t pos = line.find_last_not_of("\r\n");

        if (pos != std::string::npos)
        {
            line.erase(pos + 1);
        }
    }
}

bool Server::Authentication(Server &s, int fds_fd)
{
    // for (size_t i = 0; i < tokens.size();i++)
    //     std::cout << tokens[i] << std::endl;
    std::string cmd[3] = {"PASS", "NICK", "USER"};
    for (size_t j = 0; j < tokens.size(); j = j + 2)
    {
        int i = 0;
        for (i = 0; i < 3; i++)
        {
            if (!tokens[j].compare(cmd[i]))
                break;
        }
        switch (i)
        {
        case 0:
            // tokens[i + 1].erase(tokens[i + 1].size() - 1);
            if (!tokens[i + 1].compare(this->PASS))
                this->pass = TRUE;
            break;
        case 1:
            if (!isNickThere(s, tokens[j + 1]))
            {
                this->nick = TRUE;
                s.client[fds_fd].setNickname(tokens[j + 1]);
            }
            break;
        case 2:
            if (!checkUserCmd(tokens[j + 1]))
                this->user = TRUE;
            parseUserInfos(s, tokens[j + 1], fds_fd);
        default:
            if (!pass || !nick || !user)
            {
                std::string failure = "\033[1;31mPLEASE TRY AGAIN\033[0m\n";
                tokens.clear();
                send(fds_fd, failure.c_str(), failure.size() + 1, 0);
                return FALSE;
            }
        }
    }
    if (pass && nick && user)
    {
        char host[256];
        gethostname(host, sizeof(host));
        std::string mssg;
        mssg = std::string(":") + host + " 001 " + s.client.at(fds_fd).getNickname() + " :Welcome to Our IRC Server!, " + s.client[fds_fd].getNickname() + "\r\n";
        if (send(fds_fd, mssg.c_str(), mssg.size(), 0) == -1)
            std::perror("send error");
        mssg = std::string(":") + host + " 002 " + s.client.at(fds_fd).getNickname() + " :Your host is " + host + "\r\n";
        if (send(fds_fd, mssg.c_str(), mssg.size(), 0) == -1)
            std::perror("send error");
        this->Authen = TRUE;
        tokens.clear();
        return TRUE;
    }
    return FALSE;
}

bool Server::isNickThere(Server &s, std::string nickName)
{
    std::map<int, Client>::iterator it;
    for (it = s.client.begin(); it != s.client.end(); it++)
    {
        if (it->second.getNickname() == nickName)
            return (true);
    }
    return (false);
}

Client Server::getClientFromChannel(Server &server, std::string nickName, std::string channelName)
{
    std::map<std::string, Client> client = server.channel[channelName].channelClients;
    std::map<std::string, Client>::iterator it;
    for (it = client.begin(); it != client.end(); it++)
    {
        if (it->first == nickName)
            return it->second;
    }
    return Client();
}

bool Server::isNickInChannel(Server &server, std::string nickName, std::string channelName)
{
    std::map<std::string, Client> client = server.channel[channelName].channelClients;
    std::map<std::string, Client>::iterator it;
    for (it = client.begin(); it != client.end(); it++)
        if (it->second.getNickname() == nickName)
            return (true);
    return (false);
}

bool Server::isChannelIsThere(std::string channelName)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channel.begin(); it != channel.end(); it++)
        if (it->first == channelName)
            return (true);
    return (false);
}
//---------------------------------- Spinosa

void Server::parseUserInfos(Server &s, std::string userInfos, int fds_fd)
{
    size_t pos;
    int i = 0;
    size_t lenght;
    size_t begin = 0;
    pos = userInfos.find(" ", 0);
    while (i < 4)
    {
        lenght = pos - begin;
        if (i == 0)
            s.client[fds_fd].setUserName(userInfos.substr(begin, lenght));
        else if (i == 1)
            s.client[fds_fd].setUserMode(userInfos.substr(begin, lenght));
        else if (i == 2)
            s.client[fds_fd].setUnused(userInfos.substr(begin, lenght));
        else if (i == 3)
            s.client[fds_fd].setRealName(userInfos.substr(begin, lenght));
        begin = pos + 1;
        pos = userInfos.find(" ", 0);
        i++;
    }
}
/////////////////////////////////////////////////////////////////////////////

void Server::client_handling(Server &server, int fds_fd)
{
    // std::cout << "WELCOME TO OUR IRC" << std::endl;
    commands cmd;
    server.client[fds_fd].addVector(server, tokens, fds_fd);
    server.client[fds_fd].setFd(server, fds_fd);

    // check if clients are added
    // std::cout << "(client_Handling) CLIENTS: " << std::endl;
    // for (std::map<int, Client>::iterator it = client.begin(); it != client.end(); ++it)
    // {
    //     std::cout << it->first << " => " << it->second.getNickname() << '\n';
    // }

    if (!tokens.empty() && !tokens[0].compare("JOIN"))
        cmd.checkJoinParam(server.client[fds_fd], server);
    if (!tokens.empty() && !tokens[0].compare("KICK"))
        cmd.Kick(server.client[fds_fd], server);
    if (!tokens.empty() && !tokens[0].compare("INVITE"))
        cmd.Invite(server.client[fds_fd], server);
    if (!tokens.empty() && !tokens[0].compare("TOPIC"))
        cmd.Topic(server.client[fds_fd], server);
    if (!tokens.empty() && !tokens[0].compare("MODE"))
        cmd.Mode(server.client[fds_fd], server);
    server.client[fds_fd].tokens.clear();
    tokens.clear();
}

void Server::ft_server()
{
    Server server;
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
    if (bind(this->server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        throw std::runtime_error("Error: bind failed");

    for (int i = 0; i < FD_SETSIZE; i++)
    {
        this->client_socket.insert(client_socket.begin() + i, 0);
    }

    if (listen(this->server_fd, SOMAXCONN) < 0)
        throw std::runtime_error("Error: listen");

    std::cout << "Server listening on port 4444..." << std::endl;

    memset(fds, 0, sizeof(server.fds));

    fds[0].fd = this->server_fd;
    fds[0].events = POLLIN;

    client_fd = 0;
    while (true)
    {
        int pollResult = poll(fds, client_fd + 1, -1);
        if (pollResult == -1)
        {
            perror("Error in poll");
            break;
        }

        if (pollResult == 0)
        {
            continue;
        }
        if (fds[0].revents && POLLIN)
        {
            this->Authen = FALSE;
            this->pass = FALSE;
            this->nick = FALSE;
            this->user = FALSE;
            tokens.clear();
            this->clientSocket = accept(this->server_fd, (struct sockaddr *)&clientAddr, &addrSize);
            if (clientSocket == -1)
            {
                perror("Error accepting connection");
                continue;
            }
            if (client_fd >= 1024)
            {
                std::cout << "Maximum number of clients reached. Rejecting new connection." << std::endl;
                close(clientSocket);
            }
            else
            {
                std::cout << "New connection established. Client IP: "
                          << inet_ntoa(clientAddr.sin_addr) << ", Client Port: "
                          << ntohs(clientAddr.sin_port) << std::endl;
                fds[client_fd + 1].fd = clientSocket;
                fds[client_fd + 1].events = POLLIN;
                ++client_fd;
                client_socket.push_back(clientSocket);
            }
        }
        char buffer[1024];
        for (int i = 1; i <= client_fd; ++i)
        {
            if (fds[i].revents && POLLIN)
            {
                this->valread = recv(fds[i].fd, buffer, 1024, 0);
                if (this->valread == 0)
                {
                    std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;

                    close(fds[i].fd);
                    fds[i] = server.fds[client_fd];
                    --client_fd;
                    // client.erase(fds[i].fd);
                    continue;
                }
                else
                {
                    std::cout << buffer;
                    std::string input = buffer;
                    std::string delimiter = " ";

                    size_t pos = 0;
                    std::string token;
                    if ((pos = input.find(delimiter)) != std::string::npos)
                    {
                        token = input.substr(0, pos);
                        tokens.push_back(token);
                        input.erase(0, pos + delimiter.length());
                        tokens.push_back(input.substr(0, input.find("\n")));
                    }
                    trimCRLF(tokens);
                    if (!this->Authen && tokens.size() == 6)
                        Authentication(server, fds[i].fd);
                }
                if (this->Authen)
                    client_handling(server, fds[i].fd);
            }
        }
    }
    for (int i = 0; i < clientSocket; ++i)
    {
        close(fds[i + 1].fd);
    }

    close(this->server_fd);
}

// -------------------------------- Mountassir

Client Server::getClient(Server &s, std::string name)
{
    std::cout << "getClient Called " << std::endl;
    std::map<int, Client>::iterator it;
    for (it = s.client.begin(); it != s.client.end(); it++)
    {
        std::cout << "MAP+++" << it->second.getNickname() << std::endl;
        if (it->second.getNickname() == name)
            return it->second;
    }
    static Client defaultClient;
    return defaultClient;
}

Channel &Server::getChannelByName(std::string channelName)
{
    std::map<std::string, Channel>::iterator it;
    for (it = channel.begin(); it != channel.end(); ++it)
    {
        if (it->first == channelName)
        {
            std::cout << "Channel Name: " << it->first << std::endl;
            return it->second;
        }
    }

    static Channel defaultChannel;
    return defaultChannel;
}
