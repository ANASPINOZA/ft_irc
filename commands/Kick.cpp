/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:03 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/14 02:48:27 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void kick(std::vector<std::string> cmd, Client &kicker, Server &server)
{
    if (cmd.size() < 3)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(kicker.getNickname()) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    std::string channelName = cmd[1];
    std::string targetNickname = cmd[2];
    std::string comment = (cmd.size() > 3) ? cmd[3] : "";

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

    Channel channel = server.getChannelByName(channelName);
    Client targetClient = server.getClient(targetNickname);

    if (!server.isNickThere(targetNickname))
    {
        std::string errorMsg = ERR_NOSUCHNICK(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

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

    if (channel.removeClientFromChannel(targetClient))
    {
        channel.setUsersNum(channel.getUsersNum() - 1);
        std::string successMsg = RPL_KICK(kicker.getNickname(), targetNickname, channelName, comment) + "\r\n";
        sendMessage(successMsg, kicker.getFd());
    }
}
