#include "ChannelCommands.hpp"


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/

void    ChannelCommands::checkJoinParam(Client &client, Server &server , int i)
{
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    size_t  chanelsNum;
    size_t  keysNum;
    std::string message;
    size_t  userNum;
    std::vector<std::string> cmd;
    
    // cmd = client;
    cmd = splitStrToVec(cmd[i], ' ');
    if (cmd.size() > 1)
    {
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
                    if (channels[i][0] == '#' && channels[i][1] != '\0')
                    {
                        Channel newChannel(channels[i], client);
                        newChannel.getChannelClients()[client.getNickname()].setOP(IS_OP);
                        server.getChannels().insert(std::make_pair(channels[i], newChannel));
                        server.getChannels()[channels[i]].getChannelOperators().push_back(client.getNickname());
                        userNum = server.getChannels()[channels[i]].getUsersNum();
                        server.getChannels()[channels[i]].setUsersNum(userNum + 1);
                        server.getChannels()[channels[i]].setChannelPassword(keys[i]);

                        message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(),0))
                            std::perror("send message error");
                        message = ":" + getHostName() + " MODE " + channels[i] + " " + "\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(),0))
                            std::perror("send message error");
                        message = ":" + getHostName() + " 353 " + client.getNickname() + " = " + channels[i] + " :@" + client.getNickname() + "\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(),0))
                            std::perror("send message error");
                        message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + " :End of /NAMES list.\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(),0))
                            std::perror("send message error");
                    }
                    else
                    {
                        message = ":" + getHostName() + " 403 " + client.getNickname() + " :" + channels[i] + " No such channel\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(),0))
                            std::perror("send message error");
                        return ;
                    }

                }
                else
                {
                    // channel already exist
                    if (server.getChannels()[channels[i]].getMaxNumUsers() > server.getChannels()[channels[i]].getUsersNum())
                    {
                        if (server.getChannels()[channels[i]].getOnlyInvited() == PRIVATE_CHANNEL)
                        {
                            // if channel is private (invitation only)
                            // i should to make a function that check if the client is invited or not
                            bool isInvited = server.getChannels()[channels[i]].isClientisInvited(client.getNickname(), server);
                            if (!isInvited)
                            {
                                message = ":" + getHostName() + " 473 " + client.getNickname() + " :" + channels[i] + " Cannot join channel (+i)\r\n";
                                if(send(client.getFd(), message.c_str(), message.length(), 0))
                                    std::perror("send message error");
                            }
                            else
                            {
                                // is invited code here
                                if (server.getChannels()[channels[i]].getChannelClients().find(client.getNickname()) != server.getChannels()[channels[i]].getChannelClients().end())
                                {
                                    message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(),0))
                                        std::perror("send message error");
                                }
                                else
                                {
                                    server.getChannels()[channels[i]].getChannelClients().insert(std::make_pair(client.getNickname(), client));
                                    userNum = server.getChannels()[channels[i]].getUsersNum();
                                    server.getChannels()[channels[i]].setUsersNum(userNum + 1);
                                    message = ":" + client.getNickname() + client.getUserName() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                                    server.getChannels()[channels[i]].sendMsgToChannel(message, client.getFd());
                                    message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.getChannels()[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
                                    if(send(client.getFd(), message.c_str(), message.length(), 0))
                                        std::perror("send message error");
                                    message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(),0))
                                        std::perror("send message error");
                                }
                            }
                        }
                        else
                        {
                            // if channel is public (anyone can join)
                            if (server.getChannels()[channels[i]].getChannelPassword() != keys[i])
                            {
                                message =  ":" + getHostName() + " 464 * :Password incorrect\r\n";
                                if (send(client.getFd(), message.c_str(), message.length(),0))
                                    std::perror("send message error");
                            }
                            else
                            {
                                if (server.getChannels()[channels[i]].getChannelClients().find(client.getNickname()) != server.getChannels()[channels[i]].getChannelClients().end())
                                {
                                    message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(),0))
                                        std::perror("send message error");
                                }
                                else
                                {
                                    server.getChannels()[channels[i]].getChannelClients().insert(std::make_pair(client.getNickname(), client));
                                    userNum = server.getChannels()[channels[i]].getUsersNum();
                                    server.getChannels()[channels[i]].setUsersNum(userNum + 1);
                                    message = ":" + client.getNickname() + client.getUserName() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                                    server.getChannels()[channels[i]].sendMsgToChannel(message, client.getFd());
                                    message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.getChannels()[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
                                    if(send(client.getFd(), message.c_str(), message.length(), 0))
                                        std::perror("send message error");
                                    message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
                                    if (send(client.getFd(), message.c_str(), message.length(),0))
                                        std::perror("send message error");
                                }
                            }
                        }
                    }
                    else
                    {
                        message = ":" + getHostName() + " 471 " + client.getNickname() + " " + channels[i] + " :Cannot join channel (+l)\r\n";
                        if (send(client.getFd(), message.c_str(), message.length(),0))
                            std::perror("send message error");
                        return ;
                    }
                }

            }

        }
        else
        {
            message = ":" + getHostName() + " 400 " + client.getNickname() + "too much passwords for a channel\r\n";
            if (send(client.getFd(), message.c_str(), message.length(),0))
                std::perror("send message error");
        }
    }
    else
    {
        message = ":" + getHostName() + " 461 * " + " : " "JOIN" " Not enough parameters\r\n";
        if (send(client.getFd(), message.c_str(), message.length(),0))
			std::perror("send message error");
        return ;
    }

}

// void    ChannelCommands::checkPrivmsgParam(Client &client ,Server &server)
// {
//     //check param validation
//     std::vector<std::string> cmd;
//     std::string message;
//     int     i = 0;

//     cmd = splitVec(client.tokens, ' ');

//     if (cmd.size() > 2)
//     {
//         if (cmd[i][0] != '#')
//         {
//           // error ...
//         }
//         else
//         {
//             std::vector<std::string> users = splitStrToVec(cmd[1], ',');
//             for(size_t i = 0; i < users.size(); i++)
//             {
//                 server.
//             }
//         }
//     }
//     else
//     {
//         // error ... send a reply to the client and following the limechat syntax
//     }
// }

// int    ChannelCommands::checkCmds(std::string cmd)
// {
//     std::string cmds[6] = {"PRIVMSG", "JOIN", "INVITE", "KICK", "TOPIC", "MODE"};
//     // int        i = 0;
//     for(int i = 0; i < 5; i++)
//     {
//         if (cmds[i] == cmd)
//             return(i);
//     }
//     return (-1);
// }


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