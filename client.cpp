/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:14:11 by aadnane           #+#    #+#             */
/*   Updated: 2023/07/14 19:46:16 by aadnane          ###   ########.fr       */
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

Client &				Client::operator=( Client const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


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

/* ************************************************************************** */