/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:03 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 18:58:08 by ahel-mou         ###   ########.fr       */
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
    std::cout << targetNickname << std::endl;
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

    if (channel.removeClientFromChannel(server, userToKickInChannel, channelName))
    {
        std::string successMsg = RPL_KICK(kicker.getNickname(), targetNickname, channelName, comment) + "\r\n";
        sendMessage(successMsg, userToKickInChannel.getFd());
        sendMessageToChannel(channel, successMsg);
    }
    else
        sendMessage("ERROR : failed to kick user\r\n", kicker.getFd());
}
