#include "Commands.hpp"


void    commands::Privmsg(Client &client ,Server &server)
{
    std::vector<std::string> target;
    std::vector<std::string> cmd;
    std::map<std::string, Channel> channel;
    std::string message;

    cmd = splitVec(client.tokens, ' ');

    if (cmd.size() > 2)
    {
        target = splitStrToVec(cmd[1], ',');
        for(size_t i = 0; i < target.size(); i++)
        {
            if (target[i][0] != '#')
            {
                std::map<int, Client>clients = server.client;
                int clienFd = server.getFdOfExistedClient(target[i], server);
                if (clients.find(clienFd) != clients.end())
                {
                    message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " PRIVMSG ";
                    for(size_t j = 2; j < cmd.size(); j++)
                        message += " " + cmd[j];
                    message += "\r\n";
                    sendMessage(message, clienFd);
                }
                else
                {
                    message = ":" + getHostName() + " 401 " + client.getRealName() + " " + cmd[1] + " :user doesn't exist\r\n";
                    sendMessage(message, clienFd);
                }
            }
            else
            {
                channel = server.channel;
                if (channel.find(target[i]) != channel.end())
                {
                    if (channel[target[i]].channelClients.find(client.getNickname()) != channel[target[i]].channelClients.end())
                    {
                        message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " PRIVMSG ";
                        for(size_t k = 2; k < cmd.size(); k++)
                            message += " " + cmd[k];
                        message += "\r\n";
                        server.channel[target[i]].sendMsgToChannel(message, client.getFd());
                    }
                    else
                    {
                        message = ":" + getHostName() +  " 401 " + client.getNickname() + " " + target[1] + " :you are not in this channel\r\n";
                        sendMessage(message, client.getFd());
                    }
                }
                else
                {
                    message = ":" + getHostName() + " 401 " + client.getNickname() + " " + target[1] + " :channel doesn't exist\r\n";
                    sendMessage(message, client.getFd());
                }
            }

        }
    }
    else
    {
        message = ":" + getHostName() + " 461 " + client.getNickname() + " :PRIVMSG command requires 2 arguments\r\n";
        sendMessage(message, client.getFd());
    }
}
