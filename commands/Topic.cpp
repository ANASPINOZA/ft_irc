/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 18:59:39 by ahel-mou         ###   ########.fr       */
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

    const std::string topic = getTextAfterColon(c.getTokens()[1]);
    if (topic.empty())
    {
        if (channel.getOnlyOperatorTopic() && !channel.isOperator(c.getNickname()))
        {
            std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }
        channel.setChannelTopic("");
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, topic) + "\r\n";
        sendMessageToChannel(channel, topicMsg);
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
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, topic) + "\r\n";
        sendMessageToChannel(channel, topicMsg);
    }
}
