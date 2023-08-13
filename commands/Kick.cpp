/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:03 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/13 19:47:07 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void kick(std::vector<std::string> cmd, Client &c, Server &s)
{
    if (cmd.size() < 4)
    {
        std::cout << ERR_NEEDMOREPARAMS(c.getNickname()) << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string nickname = cmd[2];
    std::string comment = cmd[3];

    if (channelName[0] != '#')
    {
        std::cout << ERR_NOSUCHCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }


    if (s.isChannelIsThere(channelName) == false)
    {
        std::cout << ERR_NOSUCHCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    Channel channel = s.getChannelByName(channelName);

    Client userInChannel = channel.getClientInChannel(c.getNickname());
    if (userInChannel.getNickname() != c.getNickname())
    {
        std::cout << ERR_USERNOTINCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    if (s.isNickThere(nickname) == false)
    {
        std::cout << ERR_NOSUCHNICK(c.getNickname(), nickname) << std::endl;
        return;
    }

    Client client = s.getClient(nickname);
    if (client.getNickname() != nickname)
    {
        std::cout << ERR_NOSUCHNICK(c.getNickname(), nickname) << std::endl;
        return;
    }

    Client userToKickInChannel = channel.getClientInChannel(nickname);
    if (userToKickInChannel.getNickname() != nickname)
    {
        std::cout << ERR_USERNOTINCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    if (channel.isOperator(c.getNickname()) == false)
    {
        std::cout << ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) << std::endl;
        return;
    }

    if (channel.removeClientFromChannel(client))
    {
        channel.setUsersNum(channel.getUsersNum() - 1);
        if (!comment.empty())
            std::cout << RPL_KICK(c.getNickname(), nickname, channelName, comment) << std::endl;
        else
            std::cout << RPL_KICK(c.getNickname(), nickname, channelName, "") << std::endl;
    }
}