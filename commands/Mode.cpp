/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:05 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/08 00:47:58 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

std::string parseMode(const std::string &mode)
{
    std::string parsedMode;

    for (char c : mode)
    {
        if (c == '+' || c == '-')
        {
            if (parsedMode.empty() || parsedMode.back() != c)
            {
                parsedMode += c;
            }
        }
        else if (c == 't' || c == 'l' || c == 'k' || c == 'o' || c == 'i')
        {
            parsedMode += c;
        }
    }

    return parsedMode;
}

void handleMode(const std::string &mode, Channel &channel, Client &c, Client &user)
{
    if (mode.empty() || mode.size() < 2)
    {
        std::cout << "Error: Invalid mode specified. Usage: /mode <channel_name> <mode> <nickname>" << std::endl;
        return;
    }

    if (!channel.isOperator(c.getNickname()))
    {
        std::cout << "Error: You are not an operator or the owner of this channel" << std::endl;
        return;
    }

    char modeSymbol = mode[0];
    char modeOption = mode[1];

    switch (modeOption)
    {
    case 'i':
        if (modeSymbol == '+')
        {
            channel.setOnlyInvited(PRIVATE_CHANNEL);
            std::cout << "Channel is Private now, users can not join without an invitation" << std::endl;
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyInvited(PUBLIC_CHANNEL);
            std::cout << "Channel is Public now, Everyone can join without an invitation" << std::endl;
        }
        break;

    case 't':
        if (modeSymbol == '+')
        {
            channel.setOnlyOperatorTopic(true);
            std::cout << "Only operators can change the topic now" << std::endl;
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyOperatorTopic(false);
            std::cout << "All channel users can change the topic now" << std::endl;
        }
        break;

    case 'o':
        if (modeSymbol == '+')
        {
            if (channel.isOperator(user.getNickname()))
            {
                std::cout << "This user is already an operator" << std::endl;
            }
            else
            {
                channel.setChannelOperators(user.getNickname());
                std::cout << "User " << user.getNickname() << " is now an operator" << std::endl;
            }
        }
        else if (modeSymbol == '-')
        {
            if (channel.isOperator(user.getNickname()))
            {
                channel.removeOperator(user.getNickname());
                std::cout << "User " << user.getNickname() << " is no longer an operator" << std::endl;
            }
            else
            {
                std::cout << "This user is not an operator" << std::endl;
            }
        }
        break;

    case 'k':
        if (modeSymbol == '+')
        {
            channel.setProtectedByPassword(true);
            channel.setChannelPassword(mode.substr(2));
            std::cout << "Channel key is now " << mode.substr(2) << std::endl;
        }
        else if (modeSymbol == '-')
        {
            if (channel.getProtectedByPassword())
            {
                channel.setProtectedByPassword(false);
                channel.setChannelPassword("");
                std::cout << "Channel key is now empty" << std::endl;
            }
            else
            {
                std::cout << "Channel is not protected by a key" << std::endl;
            }
        }
        break;

    case 'l':
        if (modeSymbol == '+')
        {
            channel.setMaxNumUsers(std::stoi(mode.substr(2)));
            std::cout << "Channel max number of users is now " << mode.substr(2) << std::endl;
        }
        else if (modeSymbol == '-')
        {
            channel.setMaxNumUsers(0);
            std::cout << "Channel max number of users is now unlimited" << std::endl;
        }
        break;

    default:
        std::cout << "Error: Invalid mode option specified" << std::endl;
    }
}

void Mode(std::vector<std::string> &cmd, Client &c)
{
    if (cmd.size() != 4)
    {
        std::cout << "Error: Invalid number of arguments. Usage: /mode <channel_name> <mode> <nickname>" << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string mode = parseMode(cmd[2]);
    std::string nickname = cmd[3];

    if (channelName[0] != '#')
    {
        std::cout << "Error: Invalid channel name" << std::endl;
        return;
    }

    Channel channel = getChannelByName(channelName);
    if (channel.getChannelName().empty())
    {
        std::cout << "Error: Channel doesn't exist" << std::endl;
        return;
    }

    Client userInChannel = channel.getClientInChannel(c.getNickname());
    if (userInChannel.getNickname() != c.getNickname())
    {
        std::cout << "Error: You are not in this channel" << std::endl;
        return;
    }

    Client client = getUser(nickname);
    if (client.getNickname().empty())
    {
        std::cout << "Error: User doesn't exist" << std::endl;
        return;
    }

    userInChannel = channel.getClientInChannel(nickname);
    if (userInChannel.getNickname() != nickname)
    {
        std::cout << "Error: This user isn't in this channel" << std::endl;
        return;
    }

    handleMode(mode, channel, c, userInChannel);
}