#include "server.hpp"

Server::Server()
{

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

bool isValidUserCommand(const std::string &command)
{
    std::istringstream iss(command);
    std::string firstToken;
    iss >> firstToken;

    if (firstToken == "USER")
    {
        std::vector<std::string> parameters;
        std::string param;

        // Extract parameters
        while (iss >> param)
        {
            parameters.push_back(param);
        }

        // Check the number of parameters
        if (parameters.size() == 4)
        {
            return true;
        }
    }

    return false;
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

bool Server::checkPASS(Server &s, std::string param, int idx, int fds_fd)
{
    if (!param.compare(this->PASS))
        s.client[fds_fd].setPass(TRUE);
    else
    {
        Failure(s, fds_fd, idx);
        return FALSE;
    }
    return TRUE;
}

bool Server::checkNICK(Server &s, std::string nick, int fd, int idx)
{
    if (!isNickThere(s, nick))
    {
        s.client[fd].setNick(TRUE);
        s.client[fd].setNickname(nick);
    }
    else
    {
        Failure(s, fd, idx);
        return FALSE;
    }
    return TRUE;
}

bool Server::checkUSER(Server &s, std::string user, int fd, int idx)
{
    if (!checkUserCmd(user))
    {
        s.client[fd].setUser(TRUE);
        parseUserInfos(s, user, fd);
    }
    else
    {
        Failure(s, fd, idx);
        return FALSE;
    }
    return TRUE;
}

bool Server::isFdThere(Server &s, int fd)
{
    std::map<int, Client>::iterator it;
    for (it = s.client.begin(); it != s.client.end(); it++)
        if (it->first == fd)
            return (TRUE);
    return (FALSE);
}

void Server::Failure(Server &s, int fds_fd, int idx)
{
    std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;
    std::string failure = "\033[1;31mPLEASE TRY AGAIN\033[0m\n";
    send(fds_fd, failure.c_str(), failure.size() + 1, 0);
    close(fds_fd);
    if (isFdThere(s, fds_fd))
        s.client.erase(fds_fd);
    fds.erase(fds.begin() + idx);
    tokens.clear();
}

bool Server::Authentication(Server &s, int fds_fd, int idx)
{
    if (!tokens[0].compare("PASS") && !checkPASS(s, tokens[1], idx, fds_fd))
        return FALSE;
    if (!tokens[0].compare("NICK") && !checkNICK(s, tokens[1], fds_fd, idx))
        return FALSE;
    if (!tokens[0].compare("USER") && !checkUSER(s, tokens[1], fds_fd, idx))
        return FALSE;
    if (tokens[0].compare("PASS") && tokens[0].compare("USER") && tokens[0].compare("NICK"))
    {
        Failure(s, fds_fd, idx);
        tokens.clear();
        return FALSE;
    }
    tokens.clear();
    if (s.client[fds_fd].getPass() && s.client[fds_fd].getNick() && s.client[fds_fd].getUser())
    {
        std::string mssg;
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :Welcome to Our IRC Server!, " + s.client[fds_fd].getNickname() + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :       ::::    ::: :::::::::: ::::::::  ::::    :::" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :      :+:+:   :+: :+:       :+:    :+: :+:+:   :+:" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :     :+:+:+  +:+ +:+       +:+    +:+ :+:+:+  +:+" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :    +#+ +:+ +#+ +#++:++#  +#+    +:+ +#+ +:+ +#+" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :   +#+  +#+#+# +#+       +#+    +#+ +#+  +#+#+#" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :  #+#   #+#+# #+#       #+#    #+# #+#   #+#+#" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 001 " + s.client.at(fds_fd).getNickname() + " :###    #### ########## ########  ###    ####" + "\r\n";
        sendMessage(mssg, fds_fd);
        mssg = std::string(":") + getHostName() + " 002 " + s.client.at(fds_fd).getNickname() + " :Your host is " + getHostName() + "\r\n";
        sendMessage(mssg, fds_fd);
        s.client[fds_fd].setAuthen(TRUE);
        // this->Authen = TRUE;
        tokens.clear();
        return TRUE;
    }
    return TRUE;
}

int Server::getFdOfExistedClient(std::string nickName, Server &server)
{
    std::map<int, Client>::iterator it;
    for (it = server.client.begin(); it != server.client.end(); it++)
        if (it->second.getNickname() == nickName)
            return (it->first);
    return (-1);
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

void Server::client_handling(Server &server, int fds_fd, int idx)
{
    commands cmd;
    server.client[fds_fd].addVector(server, tokens, fds_fd);
    server.client[fds_fd].setIp(inet_ntoa(clientAddr.sin_addr));
    server.client[fds_fd].setFd(server, fds_fd);
    checkIfDisconnected(server);

    if (!tokens.empty() && !tokens[0].compare("JOIN"))
        cmd.Join(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("KICK"))
        cmd.Kick(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("INVITE"))
        cmd.Invite(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("TOPIC"))
        cmd.Topic(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("MODE"))
        cmd.Mode(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("PRIVMSG"))
        cmd.Privmsg(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("BOT"))
        cmd.Bot(server.client[fds_fd], server);
    else if (!tokens.empty() && !tokens[0].compare("QUIT"))
    {
        close(fds_fd);
        fds.erase(fds.begin() + idx);
    }
    else if (!tokens.empty() && !tokens[0].compare("PONG"))
        std::cout << "PING..." << std::endl;
    else if (!tokens.empty())
    {
        std::string mssg = std::string(":") + getHostName() + " 401 " + server.client.at(fds_fd).getNickname() + " :uknown command" + "\r\n";
        sendMessage(mssg, fds_fd);
    }
    server.client[fds_fd].tokens.clear();
    tokens.clear();
}

bool containsNewline(const std::string &input)
{
    return input.find('\n') != std::string::npos;
}

void   Server::ft_getHostMachine(Server &s)
{
    char *addr = inet_ntoa(clientAddr.sin_addr);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* result;
    int status = getaddrinfo(addr, NULL, &hints, &result);

    // char hostBuffer[NI_MAXHOST];
    status = getnameinfo(result->ai_addr, result->ai_addrlen,
                s.hostBuffer, sizeof(s.hostBuffer),
                NULL, 0, NI_NAMEREQD);
        if (status != 0) {
            std::cerr << "getnameinfo error: " << gai_strerror(status) << std::endl;
            return ;
    } else {
        std::cout << "Hostname: " << s.hostBuffer << std::endl;

    }
    return ;
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
        this->client_socket.insert(client_socket.begin() + i, 0);

    if (listen(this->server_fd, SOMAXCONN) < 0)
        throw std::runtime_error("Error: listen");

    std::cout << "Server listening on port " << Port << "..." << std::endl;

    pollfd clientPoll;
    clientPoll.fd = this->server_fd;
    clientPoll.events = POLLIN;
    clientPoll.revents = 0;
    fds.push_back(clientPoll);
    std::string save;
    std::string tmp;
    fcntl(server_fd, F_SETFL, O_NONBLOCK);
    while (true)
    {
        int pollResult = poll(&fds[0], fds.size(), -1);
        if (pollResult == -1)
            std::runtime_error("Error: poll failed");

        if (pollResult == 0)
            continue;
        if (fds[0].revents && POLLIN)
        {
            tokens.clear();
            this->clientSocket = accept(this->server_fd, (struct sockaddr *)&clientAddr, &addrSize);
            fcntl(clientSocket, F_SETFL, O_NONBLOCK);
            if (clientSocket == -1)
            {
                perror("Error accepting connection");
                continue;
            }
            if (fds.size() >= 1024)
            {
                std::cout << "Maximum number of clients reached. Rejecting new connection." << std::endl;
                close(clientSocket);
                break;
            }
            else
            {
                std::cout << "New connection established. Client IP: "
                          << inet_ntoa(clientAddr.sin_addr) << ", Client Port: "
                          << ntohs(clientAddr.sin_port) << std::endl;
                ft_getHostMachine(server);

                pollfd clientPoll;
                clientPoll.fd = clientSocket;
                clientPoll.events = POLLIN;
                clientPoll.revents = 0;
                fds.push_back(clientPoll);
                client_socket.push_back(clientSocket);
            }
        }
        for (size_t i = 1; i < fds.size() ; ++i)
        {
            if (fds[i].revents && POLLIN)
            {
                memset(server.client[fds[i].fd].buffer, 0, sizeof(server.client[fds[i].fd].buffer) - 1); // clearing buffer
                this->valread = recv(fds[i].fd, server.client[fds[i].fd].buffer, sizeof(server.client[fds[i].fd].buffer) - 1, 0);
                server.client[fds[i].fd].buffer[valread] = '\0';
                if (this->valread == 0)
                {
                    std::cout << "Host disconnected , ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;

                    close(fds[i].fd);
                    if (isFdThere(server, fds[i].fd))
                        server.client.erase(fds[i].fd);
                    checkIfDisconnected(server);
                    fds.erase(fds.begin() + i);
                }
                else
                {
                    std::string input = server.client[fds[i].fd].buffer;
                    tmp = save + input;
                    if (!containsNewline(tmp))
                    {
                        save = tmp;
                        break;
                    }
                    std::string delimiter = " ";

                    std::string token;
                    size_t pos = 0;
                    if ((pos = tmp.find(delimiter)) != std::string::npos || server.client[fds[i].fd].getAuthen())
                    {
                        token = tmp.substr(0, pos);
                        tokens.push_back(token);
                        tmp.erase(0, pos + delimiter.length());
                        tokens.push_back(tmp.substr(0, tmp.find("\n")));
                        tmp.clear();
                        save.clear();
                        input.clear();
                    }
                    else if (!server.client[fds[i].fd].getAuthen())
                    {
                        Failure(server, fds[i].fd, i);
                        break;
                    }
                    trimCRLF(tokens);
                    if (!server.client[fds[i].fd].getAuthen() && !Authentication(server, fds[i].fd, i))
                        break;
                    else if (server.client[fds[i].fd].getAuthen())
                        client_handling(server, fds[i].fd, i);
                }
            }
        }
    } // END of While

} // end of function

// -------------------------------- Mountassir

Client Server::getClient(Server &s, std::string name)
{
    std::map<int, Client>::iterator it;
    for (it = s.client.begin(); it != s.client.end(); it++)
    {
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
            return it->second;
    }

    static Channel defaultChannel;
    return defaultChannel;
}

// -------------------------------- Mountassir