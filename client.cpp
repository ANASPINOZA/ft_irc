/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/15 16:29:25 by ielmakhf         ###   ########.fr       */
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

std::vector<std::string> splitStrToVec(std::string str, char del)
{
    std::vector<std::string> ret;
    std::string token;
    std::istringstream tokenStream(str);
    while (getline(tokenStream, token, del))
        ret.push_back(token);
    return (ret);
}

std::vector<std::string> splitVec(std::vector<std::string> vec, char del)
{
    std::vector<std::string>ret;
    std::string holder;
    std::string token;
    for(std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
    {
        holder += *it + " ";
    }
    std::istringstream tokenStream(holder);
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

void Client::setFd(int Fd)
{
    this->fd = Fd;
}


void Client::addData(std::string &new_token)
{
    tokens.push_back(new_token);
}

void Client::addVector(std::vector<std::string> new_vector)
{
    for (size_t i = 0; i < new_vector.size(); i++)
        tokens.push_back(new_vector[i]);
}

void Client::printData()
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

void Client::setPass(bool type)
{
    this->pass = type;
}

// bool    Client::getPass()
// {
//     return (this->pass);
// }

void Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

void Client::setUserName(std::string username)
{
    this->username = username;
}

void Client::setUserMode(std::string usermode)
{
    this->usermode = usermode;
}

void Client::setUnused(std::string unused)
{
    this->unused = unused;
}

void Client::setRealName(std::string realname)
{
    this->realname = realname;
}

std::string Client::getUserName()
{
    return (this->username);
}

std::string Client::getUnused()
{
    return (this->unused);
}

std::string Client::getUserMode()
{
    return (this->usermode);
}

std::string Client::getRealName()
{
    return (this->realname);
}

// void   Client::setUser(std::string username, std::string hostname, std::string realname)
// {
//     this->user.setUsername(username);
//     this->user.setHostname(hostname);
//     this->user.setRealname(realname);
// }

std::string Client::getNickname()
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