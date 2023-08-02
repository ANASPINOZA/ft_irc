/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/02 14:12:22 by ielmakhf         ###   ########.fr       */
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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void    Client::addData(std::string &new_token)
{
    tokens.push_back(new_token);
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

bool    Client::getPass()
{
    return (this->pass);
}

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

// User            Client::getId()
// {
//     return this->id;
// }

/* ************************************************************************** */