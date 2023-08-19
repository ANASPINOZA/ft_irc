/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:00 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/19 17:41:09 by ahel-mou         ###   ########.fr       */
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

    std::string channelName = cmd[1];
    std::string nickname = cmd[0];

    std::cout << "CHANNEL : " << cmd[1] << std::endl;
    if (channelName[0] != '#')
    {
        std::cout << "+++++++++" << std::endl
                  << "ERROR : channel name must start with #" << std::endl
                  << "+++++++++" << std::endl;
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

    if (!s.isNickThere(s, nickname))
    {
        std::cout << "++++++++++++++++++++++++++" << std::endl
                  << "ERROR : user not on the server" << std::endl
                  << "++++++++++++++++++++++++++" << std::endl;
        std::string errorMsg = ERR_NOSUCHNICK(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    Channel &channel = s.getChannelByName(channelName);

    if (channel.getChannelName() != channelName) // assuming a default channel got returned
    {
        std::cout << "++++++++++++++++++++++++++" << std::endl
                  << "ERROR : failed to get channel" << std::endl
                  << "++++++++++++++++++++++++++" << std::endl;
        std::string errorMsg = ERR_NOSUCHNICK(c.getNickname(), nickname) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    Client invitedClient = s.getClient(s, nickname);

    if (channel.getMaxNumUsers() > 0 && channel.getUsersNum() >= channel.getMaxNumUsers())
    {
        std::cout << "++++++++++++++++++++++++++" << std::endl
                  << "ERROR : channel is full" << std::endl
                  << "++++++++++++++++++++++++++" << std::endl;
        std::string errorMsg = ERR_CHANNELISFULL(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    Client userAlreadyInChannel = channel.getClientInChannel(nickname);
    if (userAlreadyInChannel.getNickname() == nickname)
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
    std::cout << "++++++++++++++++++++++++++Channel conf" << std::endl
              << "channelName : " << channel.getChannelName() << std::endl
              << "channelOwner : " << channel.getChannelOwner() << std::endl
              << "channelTopic : " << channel.getChannelTopic() << std::endl
              << "channelPassword : " << channel.getChannelPassword() << std::endl
              << "onlyInvited : " << channel.getOnlyInvited() << std::endl
              << "channelIsPrivate : " << channel.getChannelIsPrivate() << std::endl
              << "alreadyHasClients : " << channel.getAlreadyHasClients() << std::endl
              << "onlyOperatorMsg : " << channel.getOnlyOperatorMsg() << std::endl
              << "onlyOperatorTopic : " << channel.getOnlyOperatorTopic() << std::endl
              << "protectedByPassword : " << channel.getProtectedByPassword() << std::endl
              << "maxNumUsers : " << channel.getMaxNumUsers() << std::endl
              << "usersNum : " << channel.getUsersNum() << std::endl
              << "++++++++++++++++++++++++++ Users" << std::endl
              << "nickname : " << invitedClient.getNickname() << std::endl
              << "inviter : " << c.getNickname() << std::endl
              << "inviter in channel : " << channel.getClientInChannel(c.getNickname()).getNickname() << std::endl
              << "++++++++++++++++++++++++++" << std::endl;

    if (!channel.addClientToChannel(invitedClient))
    {
        std::cout << "++++++++++++++++++++++++++" << std::endl
                  << "ERROR : failed to add user" << std::endl
                  << "++++++++++++++++++++++++++" << std::endl;
        std::string errorMsg = ERR_NOSUCHNICK(c.getNickname(), nickname) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }
    std::string successMsg = RPL_INVITING(c.getNickname(), nickname, channelName) + "\r\n";
    sendMessage(successMsg, c.getFd());

    // -------------------------------------------------- check users in channel
    std::map<std::string, Client> cc = channel.getChannelClients();
    std::map<std::string, Client>::iterator it;
    for (it = cc.begin(); it != cc.end(); it++)
        std::cout << "*** " << it->first << " ***" << std::endl;
}
