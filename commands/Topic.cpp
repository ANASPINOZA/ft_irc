/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/22 01:26:09 by ielmakhf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void commands::Topic(Client &c, Server &s)
{
    std::vector<std::string> cmd = splitCommand(c.getTokens()[1]);

    if (cmd.empty())
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }
    std::string channelName = cmd[0];

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

    Channel &channel = s.getChannelByName(channelName);
    Client userInChannel = channel.getClientInChannel(c.getNickname());
    if (userInChannel.getNickname() != c.getNickname())
    {
        std::string errorMsg = ERR_USERNOTINCHANNEL(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    if (cmd.size() == 1)
    {
        sendMessage(RPL_TOPIC(c.getNickname(), channelName, channel.getChannelTopic()) + "\r\n", c.getFd());
        return;
    }

    std::string topic = getTextAfterColon(c.getTokens()[1]);
    if (topic.empty())
    {
        if (channel.getOnlyOperatorTopic() && !channel.isOperator(c.getNickname()))
        {
            std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }
        channel.setChannelTopic("");
        sendMessageToChannel(channel, ERR_BADTOPIC(c.getNickname(), channelName) + "\r\n");
        return;
    }
    else
    {
        if (channel.getOnlyOperatorTopic() && !channel.isOperator(c.getNickname()))
        {
            std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }
        channel.setChannelTopic(topic);
        if (topic.find(":") == 1)
            topic = topic.substr(1);
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, topic) + "\r\n";
        sendMessageToChannel(channel, topicMsg);
    }
}
