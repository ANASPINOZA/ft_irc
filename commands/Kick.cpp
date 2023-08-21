/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:03 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 23:21:28 by ielmakhf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void commands::Kick(Client &kicker, Server &server)
{
    std::vector<std::string> cmd = splitCommand(kicker.getTokens()[1]);
    if (cmd.size() < 2)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(kicker.getNickname()) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }
    std::string channelName = cmd[0];
    std::string targetNickname = cmd[1];
    std::string comment = (cmd.size() > 2) ? getTextAfterColon(kicker.getTokens()[1]) : "";

    if (channelName[0] != '#')
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    if (!server.isChannelIsThere(channelName))
    {
        std::string errorMsg = ERR_NOSUCHCHANNEL(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    Channel &channel = server.getChannelByName(channelName);

    Client userToKickInChannel = channel.getClientInChannel(targetNickname);

    if (userToKickInChannel.getNickname() != targetNickname)
    {
        std::string errorMsg = ERR_USERNOTINCHANNEL(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    if (!channel.isOperator(kicker.getNickname()))
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    if (kicker.getNickname() == targetNickname)
    {
        sendMessage(ERR_CANTKICK(kicker.getNickname()) + "\r\n", kicker.getFd());
        return;
    }
    if (channel.removeClientFromChannel(server, userToKickInChannel, channelName))
    {
        std::string successMsg = RPL_KICK(kicker.getNickname(), channelName, kicker.getUser(), targetNickname, comment, getHostName());
        sendMessage(successMsg, userToKickInChannel.getFd());
        sendMessageToChannel(channel, successMsg);
    }
    else
        sendMessage("000 ERROR : failed to kick user\r\n", kicker.getFd());
}
