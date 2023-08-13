/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:00 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/13 19:46:38 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Invite(std::vector<std::string> cmd, Client &c, Server &s)
{
    if (cmd.size() != 3)
    {
        std::cout << ERR_NEEDMOREPARAMS(c.getNickname()) << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string nickname = cmd[2];

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

    if (s.isNickThere(nickname) == false)
    {
        std::cout << ERR_NOSUCHNICK(c.getNickname(), nickname) << std::endl;
        return;
    }

    Client userInChannel = channel.getClientInChannel(nickname);

    if (userInChannel.getNickname() == nickname)
    {
        std::cout << ERR_USERONCHANNEL(c.getNickname(), nickname) << std::endl;
        return;
    }

    Client client = s.getClient(nickname);

    if (client.getNickname() != nickname)
    {
        std::cout << ERR_NOSUCHNICK(c.getNickname(), nickname) << std::endl;
        return;
    }

    if (channel.getMaxNumUsers() > 0 && channel.getUsersNum() >= channel.getMaxNumUsers())
    {
        std::cout << ERR_CHANNELISFULL(c.getNickname(), channelName) << std::endl;
        return;
    }

    channel.setUsersNum(channel.getUsersNum() + 1);
    channel.addClientToChannel(client);
    std::cout << RPL_INVITING(c.getNickname(), nickname, channelName) << std::endl;
}
