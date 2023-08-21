/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/21 17:22:14 by ielmakhf         ###   ########.fr       */
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
    std::vector<std::string> ret;
    std::string holder;
    std::string token;
    for (std::vector<std::string>::iterator it = vec.begin(); it != vec.end(); it++)
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

void Client::setPass(bool type)
{
    this->pass = type;
}

void Client::setUser(bool type)
{
    this->user = type;
}

void Client::setNick(bool type)
{
    this->nick = type;
}

void Client::setAuthen(bool type)
{
    this->Authen = type;
}


bool Client::getPass()
{
    return this->pass;
}

bool Client::getUser()
{
    return this->user;
}

bool Client::getNick()
{
    return this->nick;
}

bool Client::getAuthen()
{
    return this->Authen;
}

void Client::setFd(Server &s, int Fd)
{
    s.client[Fd].fd = Fd;
}

void Client::addData(std::string &new_token)
{
    tokens.push_back(new_token);
}

void Client::addVector(Server &s, std::vector<std::string> new_vector, int fds_fd)
{
    for (size_t i = 0; i < new_vector.size(); i++)
        s.client[fds_fd].tokens.push_back(new_vector[i]);
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

std::string Client::getNickname()
{
    return this->nickname;
}

int Client::getOp()
{
    return (this->isOperator);
}

void Client::setOP(int state)
{
    this->isOperator = state;
}

int Client::getFd() const
{
    return (this->fd);
}

/* ************************************************************************** */