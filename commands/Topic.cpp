/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/13 19:47:45 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Topic(std::vector<std::string> cmd, Client &c, Server &s)
{
    if (cmd.size() < 3)
    {
        // Notify about missing parameters
        std::cout << ERR_NEEDMOREPARAMS(c.getNickname()) << std::endl;
        return;
    }

    // Extract command parameters
    std::string channelName = cmd[1];
    std::string topic = cmd[2];

    // Check if channel name starts with #
    if (channelName[0] != '#')
    {
        std::cout << ERR_NOSUCHCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }
    if (s.isChannelIsThere(channelName) == false)
    {
        std::cout << ERR_NOSUCHCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    Channel channel = s.getChannelByName(channelName);

    // Get the user's presence in the channel
    Client userInChannel = channel.getClientInChannel(c.getNickname());

    // Check if the user is in the channel
    if (userInChannel.getNickname() != c.getNickname())
    {
        std::cout << ERR_USERNOTINCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    // Check if user has operator privileges
    if (!channel.isOperator(c.getNickname()))
    {
        std::cout << ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) << std::endl;
        return;
    }

    // Check if the topic is the same
    if (channel.getChannelTopic() == topic)
    {
        std::cout << RPL_TOPIC(c.getNickname(), channelName, topic) << std::endl;
        return;
    }

    // Set the new topic if it's not empty
    if (topic.empty())
    {
        channel.setChannelTopic(topic);
        std::cout << RPL_NOTOPIC(c.getNickname(), channelName) << std::endl;
    }
    else
    {
        // Check if only operators can set topic
        if (channel.getOnlyOperatorTopic() && !channel.isOperator(c.getNickname()))
        {
            std::cout << ERR_CHANOPRIVSNEEDED(c.getNickname(), channelName) << std::endl;
            return;
        }

        // Set the new topic
        channel.setChannelTopic(topic);
        std::cout << RPL_TOPIC(c.getNickname(), channelName, topic) << std::endl;
    }
}
