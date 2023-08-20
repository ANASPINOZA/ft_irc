#include "Commands.hpp"


void    commands::Privmsg(Client &client ,Server &server)
{
    // check param validation
    std::vector<std::string> users;
    std::vector<std::string> cmd;
    std::map<std::string, Channel> channel;
    std::string message;

    cmd = splitVec(client.tokens, ' ');

    if (cmd.size() > 2)
    {
        users = splitStrToVec(cmd[1], ',');
        for (size_t i = 0; i < users.size(); i++)
        {
            if (cmd[i][0] != '#')
            {
                std::map<int, Client> clients = server.client;
                int clienFd = server.getFdOfExistedClient(users[i], server);
                if (clients.find(clienFd) != clients.end())
                {
                    message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " PRIVMSG ";
                    for (size_t j = 1; j < cmd.size(); j++)
                        message += " " + cmd[j];
                    message += "\r\n";
                    if (send(clienFd, message.c_str(), message.length(), 0) == -1)
                        std::perror("send message error");
                }
                else
                {
                    message = ":" + getHostName() + " 401 " + client.getRealName() + " " + cmd[1] + " :user doesn't exist\r\n";
                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                        std::perror("send message error");
                }
            }
            else
            {
                channel = server.channel;
                if (channel.find(users[i]) != channel.end())
                {
                    if (channel[users[i]].channelClients.find(client.getNickname()) != channel[users[i]].channelClients.end())
                    {
                        message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " PRIVMSG ";
                        for (size_t k = 1; k < cmd.size(); k++)
                            message += " " + cmd[k];
                        message += "\r\n";
                        server.channel[users[i]].sendMsgToChannel(message, client.getFd());
                    }
                    else
                    {
                        message = ":" + getHostName() + " 401 " + client.getNickname() + " " + cmd[1] + " :you are not in this channel\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                    }
                }
                else
                {
                    message = ":" + getHostName() + " 401 " + client.getNickname() + " " + cmd[1] + " :channel doesn't exist\r\n";
                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                        std::perror("send message error");
                }
            }
        }
    }
    else
    {
        // error ... send a reply to the client and following the limechat syntax
        message = ":" + getHostName() + " 461 " + client.getNickname() + " :PRIVMSG command requires 2 arguments\r\n";
        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
            std::perror("send message error");
    }
}
