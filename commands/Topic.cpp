/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/19 22:47:45 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void commands::Topic(Client &c, Server &s)
{
    std::vector<std::string> cmd = topicParsing(c.getTokens()[1]);
    if (cmd.size() < 1)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    std::string channelName = cmd[0];
    std::string topic;
    if (cmd.size() > 1)
        topic = cmd[1];

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

    if (topic.empty())
    {
        std::cout << "*****************" << std::endl
                  << "TOPIC param is empty" << std::endl
                  << "*****************" << std::endl;
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, channel.getChannelTopic()) + "\r\n";
        sendMessage(topicMsg, c.getFd());
    }
    else
    {
        if (channel.getOnlyOperatorTopic() && !channel.isOperator(c.getNickname()))
        {
            std::cout << "*****************" << std::endl
                      << "ERROR : your are not an op to change the topc" << std::endl
                      << "*****************" << std::endl;
            std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }
        channel.setChannelTopic(topic);
        std::string topicMsg = RPL_TOPIC(c.getNickname(), channelName, topic) + "\r\n";
        sendMessageToChannel(channel, topicMsg);
    }
}
