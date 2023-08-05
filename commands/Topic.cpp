/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:07 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/04 14:19:45 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void Topic(std::vector<std::string> cmd, Client c)
{
    if (cmd[1].empty() || cmd[2].empty() || cmd.size() < 3 || cmd.size() > 3)
    {
        std::cout << "Not enough arguments" << std::endl;
        return;
    }
    std::string channelName = cmd[1];
    std::string topic = cmd[2];

    if (channelName[0] != '#')
    {
        std::cout << "Invalid channel name" << std::endl;
        return;
    }

    // Channel channel = getChannelByName(channelName);
    // channel.setTopic(topic);
}