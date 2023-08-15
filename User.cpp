/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:23:59 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/15 15:54:21 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User()
{
}

User::User()
{
}

User::User(const User & src)
{
}



/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

User::~User()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

User &				User::operator=( User const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR---------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void    User::setUsername(std::string username)
{
    this->username = username;
}

void    User::setHostname(std::string hostname)
{
    this->hostname = hostname;
}

void    User::setRealname(std::string realname)
{
    this->realname = realname;
}

std::string     User::getUsername()
{
    return (this->username);
}

std::string     User::getHostname()
{
    return (this->hostname);
}

std::string     User::getRealname()
{
    return (this->realname);
}

/* ************************************************************************** */