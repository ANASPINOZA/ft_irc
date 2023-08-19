

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/

#include "Commands.hpp"

void commands::checkJoinParam(Client &client, Server &server)
{
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    size_t chanelsNum;
    size_t keysNum;
    std::string message;
    size_t userNum;
    std::vector<std::string> cmd;
    int i = 1;

    cmd = client.getTokens();
    cmd = splitVec(cmd, ' ');
    if (cmd.size() > 1)
    {
        std::cout << "cmd size : " << cmd.size() << std::endl;
        if (cmd.size() > 2)
        {
            channels = splitStrToVec(cmd[i], ',');
            keys = splitStrToVec(cmd[i + 1], ',');
        }
        else
        {
            channels = splitStrToVec(cmd[i], ',');
            keys.push_back("");
        }
        chanelsNum = channels.size();
        keysNum = keys.size();
        if (chanelsNum >= keysNum)
        {
            for (size_t i = 0; i < channels.size(); i++)
            {
                int isChannelThere = server.isChannelIsThere(channels[i]);
                if (!isChannelThere)
                {
                    std::cout << "channel is not exist " << std::endl;
                    std::cout << "HEEEEEEEEERE 1" << std::endl;
                    if (channels[i][0] == '#' && channels[i][1] != '\0')
                    {
                        std::cout << "channel name : " << channels[i] << std::endl;
                        Channel newChannel(channels[i], client);
                        // newChannel.getChannelClients()[client.getNickname()].setOP(IS_OP);
                        newChannel.channelClients[client.getNickname()].setOP(IS_OP);
                        // server.getChannels().insert(std::make_pair(channels[i], newChannel));
                        server.channel.insert(std::make_pair(channels[i], newChannel));
                        server.channel[channels[i]].channelOperators.push_back(client.getNickname());
                        userNum = server.channel[channels[i]].getUsersNum();
                        server.channel[channels[i]].setUsersNum(userNum + 1);
                        server.channel[channels[i]].setChannelPassword(keys[i]);

                        std::cout << "------------> " << client.getFd() << std::endl;
                        message.clear();
                        message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                        // std::cout << "-------> "<< message << std::endl;
                        // message.clear();
                        message = std::string(":") + getHostName() + " MODE " + channels[i] + " " + "\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                        // std::cout << "-------> "<< message << std::endl;
                        message = ":" + getHostName() + " 353 " + client.getNickname() + " = " + channels[i] + " :@" + client.getNickname() + "\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                        // std::cout << "-------> "<< message << std::endl;
                        message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + " :End of /NAMES list.\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                        // std::cout << "-------> "<< message << std::endl;
                    }
                    else
                    {
                        message = ":" + getHostName() + " 403 " + client.getNickname() + " :" + channels[i] + " No such channel\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                        return;
                    }
                }
                else
                {
                    // channel already exist
                    std::cout << "channel exist " << std::endl;
                    // std::cout << "HEEEEEEER" << std::endl;
                    if (server.channel[channels[i]].getMaxNumUsers() >= server.channel[channels[i]].getUsersNum() || server.channel[channels[i]].getMaxNumUsers() == 0)
                    {
                        std::cout << "HEEEEEEEEERE 0" << std::endl;
                        std::cout << "channel password 1 " << server.channel[channels[i]].getChannelPassword() << std::endl;
                        std::cout << "cmd size : " << cmd.size() << "i pos : " << i << std::endl;
                        std::cout << "password ented size :  " << keys.size() << std::endl;
                        // std::cout << "password enterd 1 " << keys[i] << std::endl;
                        if (server.channel[channels[i]].getOnlyInvited() == PRIVATE_CHANNEL)
                        {
                            // if channel is private (invitation only)
                            // i should to make a function that check if the client is invited or not
                            bool isInvited = server.channel[channels[i]].isClientisInvited(client.getNickname(), server);
                            if (!isInvited)
                            {
                                message = ":" + getHostName() + " 473 " + client.getNickname() + " :" + channels[i] + " Cannot join channel (+i)\r\n";
                                if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                    std::perror("send message error");
                            }
                            else
                            {
                                // is invited code here
                                // if (server.channel[channels[i]].getChannelClients().find(client.getNickname()) != server.channel[channels[i]].getChannelClients().end())
                                if (server.channel[channels[i]].channelClients.find(client.getNickname()) != server.channel[channels[i]].channelClients.end())
                                {
                                    message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                        std::perror("send message error");
                                }
                                else
                                {
                                    // server.channel[channels[i]].getChannelClients().insert(std::make_pair(client.getNickname(), client));
                                    server.channel[channels[i]].channelClients.insert(std::make_pair(client.getNickname(), client));
                                    userNum = server.channel[channels[i]].getUsersNum();
                                    server.channel[channels[i]].setUsersNum(userNum + 1);
                                    message = ":" + client.getNickname() + client.getUserName() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                                    server.channel[channels[i]].sendMsgToChannel(message, client.getFd());
                                    message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.channel[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                        std::perror("send message error");
                                    message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                        std::perror("send message error");
                                }
                            }
                        }
                        else
                        {
                            // if channel is public (anyone can join)
                            // std::cout << "password enterd " << keys[i] << std::endl;
                            std::cout << "channel password " << server.channel[channels[i]].getChannelPassword() << std::endl;
                            std::cout << "HEEEEEEEEERE 2" << std::endl;
                            if (keys.size() > i && server.channel[channels[i]].getChannelPassword() != keys[i])
                            {
                                std::cout << "HEEEEEEEEERE 3" << std::endl;
                                message = ":" + getHostName() + " 464 * :Password incorrect\r\n";
                                if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                    std::perror("send message error");
                            }
                            else
                            {
                                std::cout << "HEEEEEEEEERE 4" << std::endl;
                                // std::map<std::string, Client>::iterator it = server.channel[channels[i]].getChannelClients().find(client.getNickname());
                                // std::cout << "client : " << client.getNickname() <<std::endl;
                                // std::map<std::string, Client> clients = server.channel[channels[i]].getChannelClients();
                                std::map<std::string, Client> clients = server.channel[channels[i]].channelClients;

                                // std::map<std::string, Client>::iterator it = clients.begin();
                                // for (; it != clients.end(); it++)
                                // {
                                //     std::cout << "clients size " << clients.size() << std::endl;
                                //     std::cout << "user : " << it->first << std::endl;
                                // }
                                // std::cout << it
                                // std::cout << "found client already exist : " << *(clients.find(client.getNickname())) <<std::endl;
                                if (clients.find(client.getNickname()) != clients.end())
                                {
                                    // std::cout << "if he find that the new client is already exist " << std::endl;
                                    message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                        std::perror("send message error");
                                }
                                else
                                {
                                    std::cout << "client wants to join : " << client.getNickname() << std::endl;
                                    std::cout << "channel name that client wants to join : " << channels[i] << std::endl;
                                    // std::cout << "inserting new client to the existed channel" << std::endl;
                                    // server.channel[channels[i]].getChannelClients().insert(std::make_pair(client.getNickname(), client));
                                    server.channel[channels[i]].channelClients.insert(std::make_pair(client.getNickname(), client));
                                    userNum = server.channel[channels[i]].getUsersNum();
                                    server.channel[channels[i]].setUsersNum(userNum + 1);
                                    message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                                    server.channel[channels[i]].sendMsgToChannel(message, client.getFd());
                                    message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.channel[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                        std::perror("send message error");
                                    message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                                        std::perror("send message error");
                                }
                                std::cout << "HEEEEEEEEERE 5" << std::endl;
                            }
                            std::cout << "HEEEEEEEEERE 6" << std::endl;
                        }
                    }
                    else
                    {
                        message = ":" + getHostName() + " 471 " + client.getNickname() + " " + channels[i] + " :Cannot join channel (+l)\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                            std::perror("send message error");
                        return;
                    }
                }
            }
        }
        else
        {
            message = ":" + getHostName() + " 400 " + client.getNickname() + "too much passwords for a channel\r\n";
            if (send(client.getFd(), message.c_str(), message.length(), 0) == -1)
                std::perror("send message error");
        }
    }
    else
    {
        message = ":" + getHostName() + " 461 * " + " : " + "JOIN" + " Not enough parameters\r\n";
        if (send(client.getFd(), message.c_str(), message.length(), 0))
            std::perror("send message error");
        return;
    }
}

void checkPrivmsgParam(Client &client, Server &server)
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

// void    ChannelCommands::checkInviteParam(std::string param)
// {
//     //check param validation
//     std::count(param.begin(), param.end(), '#');

// }

// void    ChannelCommands::checkTokens(std::vector<std::string> cmd)
// {
//     // std::string cmds[5] = {"JOIN", "INVITE", "KICK", "TOPIC", "MODE"};
//     int     index = checkCmds(cmd[0]);

//     switch (index)
//     {
//     case 0
//         // checkPrivmsgParam(param);
//         // checkPrivmsgParam(cmd, 1);
//         break;
//     case 1
//         // checkJoinParam(param);
//     case 2
//         // carry on .....
//     default:
//         break;
//     }
// }

// void    ChannelCommands::parser(std::vector<std::string> tokens)
// {
//     int     i = 0;
//     size_t  size = tokens.size();
//     // std::vector<std::string>::iterator it = tokens.begin();
//     // while(it != tokens.end())
//     // {
//     //     checkTokens(*it, *(it + 1))
//     // }
//     while (i < size)
//     {
//         checkTokens(tokens[i], tokens[i + 1], &i);
//     }
// }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */