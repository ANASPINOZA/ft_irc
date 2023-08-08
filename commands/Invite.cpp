/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:00 by ahel-mou          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/08/08 00:14:52 by ahel-mou         ###   ########.fr       */
=======
/*   Updated: 2023/08/08 00:49:47 by ahel-mou         ###   ########.fr       */
>>>>>>> 769c737 (bfr merge)
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Invite(std::vector<std::string> cmd, Client &c)
{
    if (cmd.size() != 3)
    {
        std::cout << "Error: Invalid number of arguments. Usage: /invite <channel_name> <nickname>" << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string nickname = cmd[2];

    if (channelName[0] != '#')
    {
        std::cout << "Error: Invalid channel name. Channel name must start with '#'" << std::endl;
        return;
    }

    Channel channel = getChannelByName(channelName);

<<<<<<< HEAD
    if (channel.getChannelName().empty())
    {
        std::cout << "Error: Channel doesn't exist" << std::endl;
        return;
    }
=======
    // if (!channel)
    // {
    //     std::cout << "Channel doesn't exist" << std::endl;i
    //     return;
    // }
>>>>>>> 769c737 (bfr merge)

    Client userInChannel = channel.getClientInChannel(nickname);

    if (userInChannel.getNickname() == nickname)
    {
        std::cout << "Error: This user is already in this channel" << std::endl;
        return;
    }

    Client client = getUser(nickname);

    if (client.getNickname().empty())
    {
        std::cout << "Error: User doesn't exist" << std::endl;
        return;
    }

    if (channel.getMaxNumUsers() > 0 && channel.getUsersNum() >= channel.getMaxNumUsers())
    {
        std::cout << "Error: Channel is full" << std::endl;
        return;
    }

    channel.setUsersNum(channel.getUsersNum() + 1);
    channel.addClientToChannel(client);
    std::cout << "Client " << nickname << " has been invited to channel " << channelName << std::endl;
}
