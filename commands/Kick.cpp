/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:03 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/08 00:14:19 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void kick(std::vector<std::string> cmd, Client &c)
{
    if (cmd.size() < 4)
    {
        std::cout << "Error: Not enough arguments. Usage: /kick <channel_name> <nickname> <comment>" << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string nickname = cmd[2];
    std::string comment = cmd[3];

    if (channelName[0] != '#')
    {
        std::cout << "Error: Invalid channel name. Channel name must start with '#'" << std::endl;
        return;
    }

    Channel channel = getChannelByName(channelName);
    if (!channel)
    {
        std::cout << "Error: Channel doesn't exist" << std::endl;
        return;
    }

    Client userInChannel = channel.getClientInChannel(c.getNickname());
    if (!userInChannel)
    {
        std::cout << "Error: You are not in this channel" << std::endl;
        return;
    }
    Client client = getUser(nickname);
    if (!client)
    {
        std::cout << "Error: User doesn't exist" << std::endl;
        return;
    }
    Client userToKickInChannel = channel.getClientInChannel(nickname);
    if (!userToKickInChannel)
    {
        std::cout << "Error: The user is not in this channel" << std::endl;
        return;
    }
    if (channel.getChannelOwner() != c.getNickname())
    {
        std::cout << "Error: You are not the owner of this channel" << std::endl;
        return;
    }
    if (channel.removeClientFromChannel(client))
    {
        channel.setUsersNum(channel.getUsersNum() - 1);
        if (!comment.empty())
            std::cout << "Client " << nickname << " has been kicked from channel " << channelName << " by " << c.getNickname() << " for " << comment << std::endl;
        else
            std::cout << "Client " << nickname << " has been kicked from channel " << channelName << " by " << c.getNickname() << std::endl;
    }
}
