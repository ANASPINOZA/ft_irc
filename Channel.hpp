/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:29 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/15 15:57:41 by ielmakhf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "client.hpp"
#include "server.hpp"

#define NOT_SET 0
#define PRIVATE_CHANNEL 1 // invitation only
#define PUBLIC_CHANNEL 2  // anyone can join
#define HAS_CLIENTS 3


class Client;
class Server;

class Channel
{

private:
	std::string channelName;
	std::string channelOwner;
	std::string channelTopic;
	std::vector<std::string> channelOperators;
	std::vector<std::string> invitedList;
	std::map<std::string, Client> channelClients;
	std::string channelPassword;
	int onlyInvited;
	int channelIsPrivate;
	int alreadyHasClients;
	int onlyOperatorMsg;
	bool onlyOperatorTopic;
	bool protectedByPassword;
	size_t maxNumUsers;
	size_t usersNum;

public:
	Channel();
	Channel(std::string name, Client user);
	~Channel();

	//----------------------------------------------------------------- Getters
	std::string getChannelName() const;
	std::string getChannelOwner() const;
	std::string getChannelTopic() const;
	std::vector<std::string> getChannelOperators() const;
	std::vector<std::string> getInvitedList() const;
	int getOnlyInvited() const;
	int getChannelIsPrivate() const;
	int getAlreadyHasClients() const;
	int getOnlyOperatorMsg() const;
	size_t getMaxNumUsers() const;
	size_t getUsersNum() const;
	bool getOnlyOperatorTopic() const;
	bool getProtectedByPassword() const;
	std::string getChannelPassword() const;
	std::map<std::string, Client> getChannelClients() const;
	std::string getChannelMembers(std::string channelName, Server &server);
	bool	isClientisInvited(std::string nickname, Server& server);
	//----------------------------------------------------------------- Setters
	void setChannelName(std::string channelName);
	void setChannelOwner(std::string channelOwner);
	void setChannelTopic(std::string channelTopic);
	void setChannelOperators(std::string channelOperator);
	void setInvitedList(const std::vector<std::string> &invitedList);
	void setChannelPassword(std::string channelPassword);
	void setOnlyInvited(int onlyInvited);
	void setChannelIsPrivate(int channelIsPrivate);
	void setAlreadyHasClients(int alreadyHasClients);
	void setOnlyOperatorMsg(int onlyOperatorMsg);
	void setMaxNumUsers(size_t maxNumUsers);
	void setUsersNum(size_t usersNum);
	void setOnlyOperatorTopic(bool onlyOperatorTopic);
	void setProtectedByPassword(bool protectedByPassword);
	// void setChannelClients(std::map<std::string, Client> clients);
	//----------------------------------------------------------------- Methods
	bool addClientToChannel(Client user);
	Client getClientInChannel(std::string nickname);
	bool removeClientFromChannel(Client user);
	bool isOperator(std::string nickname);
	void removeOperator(std::string nickname);
	void sendMsgToChannel(std::string message, int fd);
	//----------------------------------------------------------------- Operators
};

#endif
