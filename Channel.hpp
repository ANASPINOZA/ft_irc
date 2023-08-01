/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:29 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/01 16:13:08 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <map>
#include "client.hpp"
#include <vector>

# define NOT_SET		0
# define PRIVATE_CAHNNEL 1
# define ONLY_INVITED 2
# define HAS_CLIENTS 3
# define ONLY_OP_MSG 4

class Channel
{

	private:
		std::string						channelName;
		std::string						channelOner;
		std::string						channellTopic;
		std::vector<std::string>		channelOperators;
		std::map<std::string, Client> 	channelClients;
		std::vector<std::string>		invitedList;
		std::string						channelPassword;
		int								onlyInvited;
		int								channelIsPrivate;
		int								alreadyHasClients;
		int								onlyOperatorMsg;
		// int								hasUsers;
		size_t							maxNumUsers;
		size_t							usersNum;
		
	public:

		Channel();
		// Channel( Channel const & src );
		Channel (std::string name, Client user);
		~Channel();
		// setChannelName(std::string channelName);
		// Channel &		operator=( Channel const & rhs );


};


#endif /* ******************************************************** Channels_H */