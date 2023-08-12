/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/12 16:46:57 by aadnane          ###   ########.fr       */
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


int    Client::checkCmds(std::string cmd)
{
    std::string cmds[6] = {"PRIVMSG", "JOIN", "INVITE", "KICK", "TOPIC", "MODE"};
    // int        i = 0;
    for(int i = 0; i < 5; i++)
    {
        if (cmds[i] == cmd)
            return(i);
    }
    return (-1);
}

void    Client::checkJoinParam(std::vector<std::string> cmd,Client &client , int i)
{
    //check param validation
    // std::count(param.begin(), param.end(), '#');
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    size_t  chanelsNum;
    size_t  keysNum;
    std::string message;
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
            for (int i = 0; i < channels.size(); i++;)
            {
                int isCannelThere = isCannelThere(channels[i], )
            }
            
        }
        else
        {
            message = ":" + getHostName() + " 400 " + client.getNickname() + "too much passwords for a channel\r\n";
            if (send(fd, message.c_str(), message.length(),0))
                std::perror("send message error");
        }
    }
    else
    {
        message = ":" + getHostName() + " 461 * " + " : " "JOIN" " Not enough parameters\r\n";
        if (send(fd, message.c_str(), message.length(),0))
			std::perror("send message error");
        return ;
    }
        
}
void    Client::checkPrivmsgParam(std::string param)
{
    //check param validation
    if (param.size() > 2)
    {
        if (param[0] != '#')
            // error ... 
        else
        {
            
        }    
    }
    else
    {
        // error ... send a reply to the client and following the limechat syntax   
    }
}

void    Client::checkInviteParam(std::string param)
{
    //check param validation
    std::count(param.begin(), param.end(), '#');
    
}

void    Client::checkTokens(std::vector<std::string> cmd)
{
    // std::string cmds[5] = {"JOIN", "INVITE", "KICK", "TOPIC", "MODE"};
    int     index = checkCmds(cmd[0]);
    
    switch (index)
    {
    case 0
        // checkPrivmsgParam(param);
        // checkPrivmsgParam(cmd, 1);
        break;
    case 1
        // checkJoinParam(param);
    case 2
        // carry on .....
    default:
        break;
    }
}

void    Client::parser(std::vector<std::string> tokens)
{
    int     i = 0;
    size_t  size = tokens.size();
    // std::vector<std::string>::iterator it = tokens.begin();
    // while(it != tokens.end())
    // {
    //     checkTokens(*it, *(it + 1))
    // }
    while (i < size)
    {
        checkTokens(tokens[i], tokens[i + 1], &i);
    }
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

// std::string     Client::getPassword()
// {
//     return this->password;
// }

// User            Client::getUser()
// {
//     return this->user;
// }

// int            Client::getFd()
// {
//     return this->fd;
// }

/* ************************************************************************** */