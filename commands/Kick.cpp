/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:03 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/19 14:54:59 by ahel-mou         ###   ########.fr       */
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
    std::string comment = (cmd.size() > 2) ? cmd[2] : "";

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
    // Client targetClient = server.getClientFromChannel(server, targetNickname, channelName);
    // std::cout << "_>>>>>>>>>" << targetClient.getNickname() << std::endl;

    // if (!server.isNickInChannel(server, targetNickname, channelName))
    // {
    //     std::string errorMsg = ERR_NOSUCHNICK(kicker.getNickname(), channelName) + "\r\n";
    //     sendMessage(errorMsg, kicker.getFd());
    //     return;
    // }

    // check if targetClient is in channel

    Client userToKickInChannel = channel.getClientInChannel(targetNickname);
    std::map<std::string, Client> cc = channel.channelClients;
    std::map<std::string, Client>::iterator it;
    for (it = cc.begin(); it != cc.end(); it++)
        std::cout << "*** " << it->first << " ***" << std::endl;
    std::cout << "TO KICK--> " << userToKickInChannel.getNickname() << " Target -----> " << targetNickname << std::endl;

    if (userToKickInChannel.getNickname() != targetNickname)
    {
        std::cout << "###################" << std::endl;
        std::string errorMsg = ERR_USERNOTINCHANNEL(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    if (!channel.isOperator(kicker.getNickname()))
    {
        std::cout << "$$$$$$$$$$$$$$" << std::endl;
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(kicker.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, kicker.getFd());
        return;
    }

    if (channel.removeClientFromChannel(server, userToKickInChannel, channelName))
    {
        std::cout << "TEST" << std::endl;
        channel.setUsersNum(channel.getUsersNum() - 1);
        std::string successMsg = RPL_KICK(kicker.getNickname(), targetNickname, channelName, comment) + "\r\n";
        sendMessage(successMsg, kicker.getFd());
    }
}
