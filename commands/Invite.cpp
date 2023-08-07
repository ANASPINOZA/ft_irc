/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:00 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/08 00:49:47 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Invite(std::vector<std::string> cmd, Client c)
{
    if (cmd[1].empty() || cmd[2].empty() || cmd.size() < 3 || cmd.size() > 3)
    {
        std::cout << "Not enough arguments" << std::endl;
        return;
    }
    std::string channelName = cmd[1];
    std::string nickname = cmd[2];

    if (channelName[0] != '#')
    {
        std::cout << "Invalid channel name" << std::endl;
        return;
    }

    // Channel channel = getChannelByName(channelName);

    // if (!channel)
    // {
    //     std::cout << "Channel doesn't exist" << std::endl;i
    //     return;
    // }

    // Client UserInChannel = channel.getClientInChannel(nickname);

    // if (UserInChannel)
    // {
    //     std::cout << "This user is already this this channel" << std::endl;
    //     return;
    // }

    // Client client = getUser(nickname);

    // if (!client)
    // {
    //     std::cout << "User doesn't exist" << std::endl;
    //     return;
    // }
    
    // if (client.getNickname().empty())
    // {
    //     channel.addClientToChannel(client);
    //      std::cout << "Client " << nickname << " has been invited to channel " << channelName << std::endl;
    //     return;
    // }
}