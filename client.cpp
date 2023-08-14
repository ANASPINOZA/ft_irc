/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/14 16:44:26 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}



/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
** --------------------------------- METHODS ----------------------------------
*/


// int    Client::checkCmds(std::string cmd)
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

// void    Client::checkJoinParam(std::vector<std::string> cmd,Client &client, Server &server , int i)
// {
//     std::vector<std::string> channels;
//     std::vector<std::string> keys;
//     size_t  chanelsNum;
//     size_t  keysNum;
//     std::string message;
//     size_t  userNum;
    
//     cmd = splitStrToVec(cmd[i], ' ');
//     if (cmd.size() > 1)
//     {
//         if (cmd.size() > 2)
//         {
//             channels = splitStrToVec(cmd[i], ',');
//             keys = splitStrToVec(cmd[i + 1], ',');
//         }
//         else
//         {
//             channels = splitStrToVec(cmd[i], ',');
//             keys.push_back("");
//         }
//         chanelsNum = channels.size();
//         keysNum = keys.size();
//         if (chanelsNum >= keysNum)
//         {
//             for (int i = 0; i < channels.size(); i++)
//             {
//                 int isChannelThere = server.isChannelIsThere(channels[i]);
//                 if (!isChannelThere)
//                 {
//                     if (channels[i][0] == '#' && channels[i][1] != '\0')
//                     {
//                         Channel newChannel(channels[i], client);
//                         newChannel.getChannelClients()[client.getNickname()].isOperator = IS_OP;
//                         server.getChannels().insert(std::make_pair(channels[i], newChannel));
//                         server.getChannels()[channels[i]].getChannelOperators().push_back(client.getNickname());
//                         userNum = server.getChannels()[channels[i]].getUsersNum();
//                         server.getChannels()[channels[i]].setUsersNum(userNum + 1);
//                         server.getChannels()[channels[i]].setChannelPassword(keys[i]);

//                         message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
//                         if (send(fd, message.c_str(), message.length(),0))
//                             std::perror("send message error");
//                         message = ":" + getHostName() + " MODE " + channels[i] + " " + "\r\n";
//                         if (send(fd, message.c_str(), message.length(),0))
//                             std::perror("send message error");
//                         message = ":" + getHostName() + " 353 " + client.getNickname() + " = " + channels[i] + " :@" + client.getNickname() + "\r\n";
//                         if (send(fd, message.c_str(), message.length(),0))
//                             std::perror("send message error");
//                         message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + " :End of /NAMES list.\r\n";
//                         if (send(fd, message.c_str(), message.length(),0))
//                             std::perror("send message error");
//                     }
//                     else
//                     {
//                         message = ":" + getHostName() + " 403 " + client.getNickname() + " :" + channels[i] + " No such channel\r\n";
//                         if (send(fd, message.c_str(), message.length(),0))
//                             std::perror("send message error");
//                         return ;
//                     }
                    
//                 }
//                 else
//                 {
//                     // channel already exist
//                     if (server.getChannels()[channels[i]].getMaxNumUsers() > server.getChannels()[channels[i]].getUsersNum())
//                     {
//                         if (server.getChannels()[channels[i]].getOnlyInvited() == PRIVATE_CHANNEL)
//                         {
//                             // if channel is private (invitation only)
//                             // ishould to make a function that check if the client is invited or not
//                             bool isInvited = server.getChannels()[channels[i]].isClientisInvited(client.getNickname(), server);
//                             if (!isInvited)
//                             {
//                                 message = ":" + getHostName() + " 473 " + client.getNickname() + " :" + channels[i] + " Cannot join channel (+i)\r\n";
//                                 if(send(fd, message.c_str(), message.length(), 0))
//                                     std::perror("send message error");
//                             }
//                             else
//                             {
//                                 // is invited code here
//                                 if (server.getChannels()[channels[i]].getChannelClients().find(client.getNickname()) != server.getChannels()[channels[i]].getChannelClients().end())
//                                 {
//                                     message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
//                                     if (send(fd, message.c_str(), message.length(),0))
//                                         std::perror("send message error");
//                                 }
//                                 else
//                                 {
//                                     server.getChannels()[channels[i]].getChannelClients().insert(std::make_pair(client.getNickname(), client));
//                                     userNum = server.getChannels()[channels[i]].getUsersNum();
//                                     server.getChannels()[channels[i]].setUsersNum(userNum + 1);
//                                     message = ":" + client.getNickname() + client.getUserName() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
//                                     server.getChannels()[channels[i]].sendMsgToChannel(message, client.fd);
//                                     message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.getChannels()[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
//                                     if(send(fd, message.c_str(), message.length(), 0))
//                                         std::perror("send message error");
//                                     message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
//                                     if (send(fd, message.c_str(), message.length(),0))
//                                         std::perror("send message error");
//                                 }
//                             }
//                         }
//                         else
//                         {
//                             // if channel is public (anyone can join)
//                             if (server.getChannels()[channels[i]].getChannelPassword() != keys[i])
//                             {
//                                 message =  ":" + getHostName() + " 464 * :Password incorrect\r\n";
//                                 if (send(client.fd, message.c_str(), message.length(),0))
//                                     std::perror("send message error");
//                             }
//                             else
//                             {
//                                 if (server.getChannels()[channels[i]].getChannelClients().find(client.getNickname()) != server.getChannels()[channels[i]].getChannelClients().end())
//                                 {
//                                     message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
//                                     if (send(fd, message.c_str(), message.length(),0))
//                                         std::perror("send message error");
//                                 }
//                                 else
//                                 {
//                                     server.getChannels()[channels[i]].getChannelClients().insert(std::make_pair(client.getNickname(), client));
//                                     userNum = server.getChannels()[channels[i]].getUsersNum();
//                                     server.getChannels()[channels[i]].setUsersNum(userNum + 1);
//                                     message = ":" + client.getNickname() + client.getUserName() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
//                                     server.getChannels()[channels[i]].sendMsgToChannel(message, client.fd);
//                                     message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.getChannels()[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
//                                     if(send(fd, message.c_str(), message.length(), 0))
//                                         std::perror("send message error");
//                                     message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
//                                     if (send(fd, message.c_str(), message.length(),0))
//                                         std::perror("send message error");
//                                 }
//                             }
//                         }
//                     }
//                     else
//                     {
//                         message = ":" + getHostName() + " 471 " + client.getNickname() + " " + channels[i] + " :Cannot join channel (+l)\r\n";
//                         if (send(fd, message.c_str(), message.length(),0))
//                             std::perror("send message error");
//                         return ;
//                     }
//                 }
                
//             }
            
//         }
//         else
//         {
//             message = ":" + getHostName() + " 400 " + client.getNickname() + "too much passwords for a channel\r\n";
//             if (send(fd, message.c_str(), message.length(),0))
//                 std::perror("send message error");
//         }
//     }
//     else
//     {
//         message = ":" + getHostName() + " 461 * " + " : " "JOIN" " Not enough parameters\r\n";
//         if (send(fd, message.c_str(), message.length(),0))
// 			std::perror("send message error");
//         return ;
//     }
        
// }

// //--------------------------------- spinosa

// std::string     Server::getChannelMembers(std::string channelName, Server &server)
// {
//     std::string members = ":",  operators = "";
//     std::map<std::string, Client>::iterator it = server.getChannels()[channelName].getChannelClients().begin();

//     for (;it != server.getChannels()[channelName].getChannelClients().end(); it++)
//     {
//         if (it->second.getOp() != IS_OP)
//         {}
//         // if (it->first == channelName)
//         // {
//         //     std::map<std::string, Client>::iterator it2;
//         //     for (it2 = it->second.getClients().begin(); it2 != it->second.getClients().end(); it2++)
//         //     {
//         //         members += it2->first + " ";
//         //     }
//         // }
        
//     }
//     return members;
// }

// std::string IrcServer::getChannelUsers(std::string channelname)
// {
//     std::string userlist = ":";
//     std::string useroperator = "";
//     std::map<std::string, Client_irc*>::iterator it = mapchannels[channelname].clients.begin();
//     while(it != mapchannels[channelname].clients.end())
//     {
//         if (it->second->get_operator())
//             useroperator += it->second->get_nick() + " ";
//         else
//             userlist += it->second->get_nick() + " ";
//         it++;
//     }
//     if (!userlist.empty())
//         userlist += "@" + useroperator;
//     return userlist;
// }

// void    Client::checkPrivmsgParam(std::string param)
// {
//     //check param validation
//     if (param.size() > 2)
//     {
//         if (param[0] != '#')
//             // error ... 
//         else
//         {
            
//         }    
//     }
//     else
//     {
//         // error ... send a reply to the client and following the limechat syntax   
//     }
// }

// void    Client::checkInviteParam(std::string param)
// {
//     //check param validation
//     std::count(param.begin(), param.end(), '#');
    
// }

// void    Client::checkTokens(std::vector<std::string> cmd)
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

// void    Client::parser(std::vector<std::string> tokens)
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


std::vector<std::string> splitStrToVec(std::string str, char del)
{
    std::vector<std::string> ret;
    std::string token;
    std::istringstream tokenStream(str);
    while (getline(tokenStream, token, del))
        ret.push_back(token);
    return (ret);
}

std::string getHostName()
{
    char hostNames[256];
    if (!gethostname(hostNames, sizeof(hostNames)))
        return (hostNames);
    return "";
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void    Client::addData(std::string &new_token)
{
    tokens.push_back(new_token);
}

void    Client::addVector(std::vector<std::string> new_vector)
{
    for (size_t i = 0; i < new_vector.size(); i++)
        tokens.push_back(new_vector[i]);
}

void    Client::printData()
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens[i] << std::endl;
    }
}

std::vector<std::string> Client::getTokens()
{
    return (this->tokens);
}

void    Client::setPass(bool type)
{
    this->pass = type;
}

// bool    Client::getPass()
// {
//     return (this->pass);
// }

void    Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

void    Client::setUserName(std::string username)
{
    this->username = username;
}

void    Client::setUserMode(std::string usermode)
{
    this->usermode = usermode;
}

void    Client::setUnused(std::string unused)
{
    this->unused = unused;
}

void    Client::setRealName(std::string realname)
{
    this->realname = realname;
}

std::string     Client::getUserName()
{
    return (this->username);
}

std::string     Client::getUnused()
{
    return (this->unused);
}

std::string     Client::getUserMode()
{
    return (this->usermode);
}

std::string     Client::getRealName()
{
    return (this->realname);
}

// void   Client::setUser(std::string username, std::string hostname, std::string realname)
// {
//     this->user.setUsername(username);
//     this->user.setHostname(hostname);
//     this->user.setRealname(realname);
// }

std::string     Client::getNickname()
{
    return this->nickname;
}

int        Client::getOp()
{
    return (this->isOperator);
}

void        Client::setOP(int state)
{
    this->isOperator = state;
}

// std::string     Client::getPassword()
// {
//     return this->password;
// }

// User            Client::getUser()
// {
//     return this->user;
// }

int            Client::getFd()
{
    return (this->fd);
}

/* ************************************************************************** */