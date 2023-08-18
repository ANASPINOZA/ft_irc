/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:00 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/18 17:34:49 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void commands::Invite(Client &c, Server &s)
{
    std::vector<std::string> cmd = splitCommand(c.getTokens()[1]);
    if (cmd.size() != 2)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    std::string channelName = cmd[0];
    std::string nickname = cmd[1];

    if (channelName[0] != '#')
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    if (!s.isChannelIsThere(channelName))
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    Channel &channel = s.getChannelByName(channelName); // reference for makeing changes directly to the server

    if (channel.getChannelName() != nickname)
    {
        std::string errorMsg = ERR_NOSUCHNICK(c.getNickname(), nickname) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }
    if (!s.isNickThere(nickname))
    {
        std::string errorMsg = ERR_NOSUCHNICK(c.getNickname(), nickname) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    Client invitedClient = s.getClient(nickname);

    if (channel.getMaxNumUsers() > 0 && channel.getUsersNum() >= channel.getMaxNumUsers())
    {
        std::string errorMsg = ERR_CHANNELISFULL(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    Client userInChannel = channel.getClientInChannel(nickname);
    if (userInChannel.getNickname() == nickname)
    {
        std::string errorMsg = ERR_USERONCHANNEL(c.getNickname(), nickname) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    if (!channel.isOperator(c.getNickname()))
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    channel.setUsersNum(channel.getUsersNum() + 1);
    if (channel.addClientToChannel(invitedClient))
    {
        std::string successMsg = RPL_INVITING(c.getNickname(), nickname, channelName) + "\r\n";
        sendMessage(successMsg, c.getFd());
    }
}
