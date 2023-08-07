/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/08 00:14:09 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Topic.cpp
#include "Commands.hpp"

void Topic(std::vector<std::string> cmd, Client &c)
{
    if (cmd.size() < 3)
    {
        std::cout << "Error: Insufficient arguments. Usage: /topic <channel_name> <topic>" << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string topic = cmd[2];

    if (channelName[0] != '#')
    {
        std::cout << "Error: Invalid channel name. Channel name must start with '#'" << std::endl;
        return;
    }

    Channel channel = getChannelByName(channelName);
    if (channel.getChannelName().empty())
    {
        std::cout << "Error: Channel doesn't exist" << std::endl;
        return;
    }

    Client &userInChannel = channel.getClientInChannel(c.getNickname());

    if (userInChannel.getNickname() != c.getNickname())
    {
        std::cout << "Error: You are not in this channel" << std::endl;
        return;
    }
    if (!channel.isOperator(c.getNickname()))
    {
        std::cout << "Error: You are not allowed to change the topic of this channel" << std::endl;
        return;
    }
    if (channel.getChannelTopic() == topic)
    {
        std::cout << "Error: Topic of channel " << channelName << " is already " << topic << std::endl;
        return;
    }
    if (topic.empty())
    {
        channel.setChannelTopic(topic);
        std::cout << "Topic of channel " << channelName << " has been removed by " << c.getNickname() << std::endl;
    }
    else
    {
        if (channel.getOnlyOperatorTopic() && !channel.isOperator(c.getNickname()))
        {
            std::cout << "Error: You are not allowed to change the topic of this channel" << std::endl;
            return;
        }
        channel.setChannelTopic(topic);
        std::cout << "Topic of channel " << channelName << " has been changed to " << topic << std::endl;
    }
}
