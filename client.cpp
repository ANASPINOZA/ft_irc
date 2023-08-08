/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/07 19:14:08 by aadnane          ###   ########.fr       */
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
    std::string cmds[5] = {"JOIN", "INVITE", "KICK", "TOPIC", "MODE"};
    // int        i = 0;
    for(int i = 0; i < 5; i++)
    {
        if (cmds[i] == cmd)
            return(i);
    }
    return (-1);
}

void    Client::checkJoinParam(std::string param)
{
    //check param validation
    std::count(param.begin(), param.end(), '#');
}

void    Client::checkInviteParam(std::string param)
{
    //check param validation
    std::count(param.begin(), param.end(), '#');
}

void    Client::checkTokens(std::string cmd, std::string param, int *i)
{
    // std::string cmds[5] = {"JOIN", "INVITE", "KICK", "TOPIC", "MODE"};
    int     index = checkCmds(cmd);
    
    switch (index)
    {
    case 0
        // checkJoinParam(param);
        break;
    case 1
        // carry on ....
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

// void    Client::setNickname(std::string nickname)
// {
//     this->nickname = nickname;
// }

// void   Client::setUser(std::string username, std::string hostname, std::string realname)
// {
//     this->user.setUsername(username);
//     this->user.setHostname(hostname);
//     this->user.setRealname(realname);
// }

// std::string     Client::getNickname()
// {
//     return this->nickname;
// }

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