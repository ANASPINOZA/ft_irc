/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:40 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/05 15:34:49 by ahel-mou         ###   ########.fr       */
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
	channelOwner = user.getNickname();
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
//----------------------------------------------------------------- Getters
std::string Channel::getChannelName() const
{
	return channelName;
}

std::string Channel::getChannelOwner() const
{
	return channelOwner;
}

std::string Channel::getChannelTopic() const
{
	return channelTopic;
}

std::vector<std::string> Channel::getChannelOperators() const
{
	return channelOperators;
}

std::vector<std::string> Channel::getInvitedList() const
{
	return invitedList;
}

int Channel::getOnlyInvited() const
{
	return onlyInvited;
}

int Channel::getChannelIsPrivate() const
{
	return channelIsPrivate;
}

int Channel::getAlreadyHasClients() const
{
	return alreadyHasClients;
}

int Channel::getOnlyOperatorMsg() const
{
	return onlyOperatorMsg;
}

size_t Channel::getMaxNumUsers() const
{
	return maxNumUsers;
}

size_t Channel::getUsersNum() const
{
	return usersNum;
}
//----------------------------------------------------------------- Setters

void Channel::setChannelName(std::string channelName)
{
	this->channelName = channelName;
}

void Channel::setChannelOwner(std::string channelOwner)
{
	this->channelOwner = channelOwner;
}

void Channel::setChannelTopic(std::string channelTopic)
{
	this->channelTopic = channelTopic;
}

void Channel::setChannelOperators(const std::vector<std::string> &channelOperators)
{
	this->channelOperators = channelOperators;
}

void Channel::setInvitedList(const std::vector<std::string> &invitedList)
{
	this->invitedList = invitedList;
}

void Channel::setChannelPassword(std::string channelPassword)
{
	this->channelPassword = channelPassword;
}

void Channel::setOnlyInvited(int onlyInvited)
{
	this->onlyInvited = onlyInvited;
}

void Channel::setChannelIsPrivate(int channelIsPrivate)
{
	this->channelIsPrivate = channelIsPrivate;
}

void Channel::setAlreadyHasClients(int alreadyHasClients)
{
	this->alreadyHasClients = alreadyHasClients;
}

void Channel::setOnlyOperatorMsg(int onlyOperatorMsg)
{
	this->onlyOperatorMsg = onlyOperatorMsg;
}

void Channel::setMaxNumUsers(size_t maxNumUsers)
{
	this->maxNumUsers = maxNumUsers;
}

void Channel::setUsersNum(size_t usersNum)
{
	this->usersNum = usersNum;
}

//----------------------------------------------------------------- Methods

bool Channel::addClientToChannel(Client user)
{
	if (usersNum >= maxNumUsers)
	{
		return false;
	}

	std::string nickname = user.getNickname();

	if (channelClients.find(nickname) != channelClients.end())
	{
		return false;
	}

	channelClients[nickname] = user;
	usersNum++;

	return true;
}

Client Channel::getClientInChannel(std::string nickname)
{
	return channelClients[nickname];
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */