/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 16:47:29 by aadnane           #+#    #+#             */
/*   Updated: 2023/07/31 17:32:56 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <map>
#include "client.hpp"
#include <vector>

class Channel
{

	private:
		std::string						channelName;
		std::string						channelOner;
		std::string						channellTopic;
		std::vector<std::string>		channelOperators;
		std::map<std::string, Client>	channelClients;
		std::map<std::string, Client>	channelRegularUsers;
		std::vector<std::string>		invitedList;
		int								onlyInvited;
		std::string						channelPassword;
		
	public:

		Channel();
		Channel( Channel const & src );
		~Channel();

		Channel &		operator=( Channel const & rhs );


};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ******************************************************** Channels_H */