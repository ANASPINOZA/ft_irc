/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:29 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/04 12:07:12 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "Client.hpp"

#define NOT_SET 0
#define PRIVATE_CHANNEL 1
#define ONLY_INVITED 2
#define HAS_CLIENTS 3
#define ONLY_OP_MSG 4

class Channel
{

private:
	std::string channelName;
	std::string channelOwner;
	std::string channelTopic;
	std::vector<std::string> channelOperators;
	std::map<std::string, Client> channelClients;
	std::vector<std::string> invitedList;
	std::string channelPassword;
	int onlyInvited;
	int channelIsPrivate;
	int alreadyHasClients;
	int onlyOperatorMsg;
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

	//----------------------------------------------------------------- Setters
	void setChannelName(std::string channelName);
	void setChannelOwner(std::string channelOwner);
	void setChannelTopic(std::string channelTopic);
	void setChannelOperators(const std::vector<std::string> &channelOperators);
	void setInvitedList(const std::vector<std::string> &invitedList);
	void setChannelPassword(std::string channelPassword);
	void setOnlyInvited(int onlyInvited);
	void setChannelIsPrivate(int channelIsPrivate);
	void setAlreadyHasClients(int alreadyHasClients);
	void setOnlyOperatorMsg(int onlyOperatorMsg);
	void setMaxNumUsers(size_t maxNumUsers);
	void setUsersNum(size_t usersNum);

	//----------------------------------------------------------------- Methods
	bool addClientToChannel(Client user);
	Client getClientByName(std::string nickname);
};

#endif
