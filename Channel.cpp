/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:40 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/18 18:49:07 by ielmakhf         ###   ########.fr       */
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
	maxNumUsers = 256;
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

bool Channel::getOnlyOperatorTopic() const
{
	return onlyOperatorTopic;
}

size_t Channel::getMaxNumUsers() const
{
	return maxNumUsers;
}

size_t Channel::getUsersNum() const
{
	return usersNum;
}

bool Channel::getProtectedByPassword() const
{
	return protectedByPassword;
}

std::map<std::string, Client> Channel::getChannelClients() const
{
	return (channelClients);
}

std::string Channel::getChannelPassword() const
{
	return (channelPassword);
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

void Channel::setChannelOperators(std::string channelOperator)
{
	this->channelOperators.push_back(channelOperator);
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
void Channel::setOnlyOperatorTopic(bool onlyOperatorTopic)
{
	this->onlyOperatorTopic = onlyOperatorTopic;
}

void Channel::setProtectedByPassword(bool protectedByPassword)
{
	this->protectedByPassword = protectedByPassword;
}

// void Channel::setChannelClients(std::map<std::string, Client> clients)
// {
// 	this->channelClients = clients;
// }
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
	std::map<std::string, Client>::iterator it;
	for (it = channelClients.begin(); it != channelClients.end(); it++)
		std::cout << "_: " << it->first << " _:" << std::endl;
	usersNum++;

	return true;
}

Client Channel::getClientInChannel(std::string nickname)
{
	std::map<std::string, Client>::iterator it = channelClients.find(nickname);
	if (it != channelClients.end())
	{
		return it->second;
	}
	return Client();
}

bool Channel::removeClientFromChannel(Server &server, Client user, std::string channelName)
{
	std::string nickname = user.getNickname();

	std::cout << "niiiiick1 " << nickname << std::endl;
	std::map<std::string, Client> client = server.channel[channelName].channelClients;
	std::map<std::string, Client> client2 = client;
	std::map<std::string, Client>::iterator it2;
	for (it2 = client2.begin(); it2 != client2.end(); it2++)
		std::cout << "| " << it2->first << " |" << std::endl;
	if (client.find(nickname) == client.end())
	{
		return false;
	}

	server.channel[channelName].channelClients.erase(nickname);
	std::map<std::string, Client> client1 = client;
	std::map<std::string, Client>::iterator it;
	for (it = client1.begin(); it != client1.end(); it++)
		std::cout << "| " << it->first << " |" << std::endl;
	usersNum--;

	return true;
}

bool Channel::isOperator(std::string nickname)
{
	for (std::vector<std::string>::iterator it = channelOperators.begin(); it != channelOperators.end(); ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void Channel::removeOperator(std::string nickname)
{
	for (std::vector<std::string>::iterator it = channelOperators.begin(); it != channelOperators.end(); ++it)
	{
		if (*it == nickname)
		{
			channelOperators.erase(it);
			return;
		}
	}
}

void Channel::sendMsgToChannel(std::string message, int fd)
{
	for (std::map<std::string, Client>::iterator it = channelClients.begin(); it != channelClients.end(); it++)
	{
		if (fd != it->second.getFd())
		{
			if (send(it->second.getFd(), message.c_str(), message.length(), 0) == -1)
				std::perror("send message error");
		}
	}
}

std::string Channel::getChannelMembers(std::string channelName, Server &server)
{
	std::string members = ":", operators = "";
	std::map<std::string, Client> client = server.getChannels()[channelName].getChannelClients();
	std::map<std::string, Client>::iterator it = client.begin();
	for (; it != client.end(); it++)
	{
		if (it->second.getOp() != IS_OP)
		{
			members += it->second.getNickname() + " ";
		}
		else
		{
			operators += it->second.getNickname() + " ";
		}
	}
	if (!members.empty())
		members += "@" + operators;
	return (members);
}

bool Channel::isClientisInvited(std::string nickname, Server &server)
{
	std::vector<std::string> invitedUsers = server.getChannels()[channelName].getInvitedList();
	std::vector<std::string>::iterator it = invitedUsers.begin();
	for (; it != invitedUsers.end(); it++)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */