/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:40 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/01 16:19:27 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

// Channel::Channel( const Channel & src )
// {
// }

Channel::Channel(std::string name, Client user)
{
	channelName = name;
	channelOner = user.getNickname();
	onlyInvited = NOT_SET;
	channelIsPrivate = NOT_SET;
	alreadyHasClients = HAS_CLIENTS;
	usersNum = NOT_SET;
	channelClients[user.getNickname()] = user;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
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


/* ************************************************************************** */