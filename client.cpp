/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/01 16:06:11 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client()
{
}

Client::Client()
{
}

Client::Client(const Client & src)
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

void    Client::setPassword(std::string password)
{
    this->password = password;
}

void    Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

void   Client::setUser(std::string username, std::string hostname, std::string realname)
{
    this->user.setUsername(username);
    this->user.setHostname(hostname);
    this->user.setRealname(realname);
}

std::string     Client::getNickname()
{
    return this->nickname;
}

std::string     Client::getPassword()
{
    return this->password;
}

User            Client::getUser()
{
    return this->user;
}

User            Client::getId()
{
    return this->id;
}

/* ************************************************************************** */