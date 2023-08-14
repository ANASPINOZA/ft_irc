/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/14 02:33:53 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Topic(std::vector<std::string> cmd, Client &c, Server &s)
{
    if (cmd.size() < 2)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    std::string channelName = cmd[1];
    std::string topic;

    if (cmd.size() > 2)
        topic = cmd[2];

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

    Channel channel = s.getChannelByName(channelName);

    Client userInChannel = channel.getClientInChannel(c.getNickname());
    if (userInChannel.getNickname() != c.getNickname())
    {
        std::string errorMsg = ERR_USERNOTINCHANNEL(c.getNickname(), channelName) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    if (topic.empty())
    {
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, channel.getChannelTopic()) + "\r\n";
        sendMessage(topicMsg, c.getFd());
    }
    else
    {
        if (!channel.isOperator(c.getNickname()))
        {
            std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }

        if (channel.getChannelTopic() == topic)
        {
            std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, topic) + "\r\n";
            sendMessage(topicMsg, c.getFd());
            return;
        }

        channel.setChannelTopic(topic);
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, topic) + "\r\n";
        sendMessage(topicMsg, c.getFd());
    }
}
