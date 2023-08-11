/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:05 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/11 19:25:03 by ahel-mou         ###   ########.fr       */
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
        std::cout << ERR_NEEDMOREPARAMS(c.getNickname()) << std::endl;
        return;
    }

    if (!channel.isOperator(c.getNickname()))
    {
        std::cout << ERR_CHANOPRIVSNEEDED(c.getNickname(), channel.getChannelName()) << std::endl;
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
            std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption) << std::endl;
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyInvited(PUBLIC_CHANNEL);
            std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) << std::endl;
        }
        break;

    case 't':
        if (modeSymbol == '+')
        {
            channel.setOnlyOperatorTopic(true);
            std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption) << std::endl;
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyOperatorTopic(false);
            std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) << std::endl;
        }
        break;

    case 'o':
        if (modeSymbol == '+')
        {
            if (channel.isOperator(user.getNickname()))
            {
                std::cout << ERR_USERONCHANNEL(c.getNickname(), user.getNickname()) << std::endl;
            }
            else
            {
                channel.setChannelOperators(user.getNickname());
                std::cout << RPL_UMODEIS(c.getNickname(), "+" + modeOption) << std::endl;
            }
        }
        else if (modeSymbol == '-')
        {
            if (channel.isOperator(user.getNickname()))
            {
                channel.removeOperator(user.getNickname());
                std::cout << RPL_UMODEIS(c.getNickname(), "-" + modeOption) << std::endl;
            }
            else
            {
                std::cout << ERR_USERNOTINCHANNEL(c.getNickname(), user.getNickname()) << std::endl;
            }
            break;

        case 'k':
            if (modeSymbol == '+')
            {
                channel.setProtectedByPassword(true);
                channel.setChannelPassword(mode.substr(2));
                std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption + " " + mode.substr(2)) << std::endl;
            }
            else if (modeSymbol == '-')
            {
                if (channel.getProtectedByPassword())
                {
                    channel.setProtectedByPassword(false);
                    channel.setChannelPassword("");
                    std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) << std::endl;
                }
            }
            else
            {
                std::cout << ERR_KEYSET(c.getNickname(), channel.getChannelName()) << std::endl;
            }
        }
        break;

    case 'l':
        if (modeSymbol == '+')
        {
            channel.setMaxNumUsers(std::stoi(mode.substr(2)));
            std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption + " " + mode.substr(2)) << std::endl;
        }
        else if (modeSymbol == '-')
        {
            channel.setMaxNumUsers(0);
            std::cout << RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) << std::endl;
        }
        break;

    default:
        std::cout << ERR_UNKNOWNMODE(c.getNickname(), modeOption) << std::endl;
    }
}

void Mode(std::vector<std::string> &cmd, Client &c, Server &s)
{
    if (cmd.size() != 4)
    {
        std::cout << ERR_NEEDMOREPARAMS(c.getNickname()) << std::endl;
        return;
    }

    std::string channelName = cmd[1];
    std::string mode = parseMode(cmd[2]);
    std::string nickname = cmd[3];

    if (channelName[0] != '#')
    {
        std::cout << ERR_NOSUCHCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    Channel channel = getChannelByName(channelName);
    if (channel.getChannelName().empty())
    {
        std::cout << ERR_NOSUCHCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    Client userInChannel = channel.getClientInChannel(c.getNickname());
    if (userInChannel.getNickname() != c.getNickname())
    {
        std::cout << ERR_USERNOTINCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    if (s.isNickThere(nickname) == false)
    {
        std::cout << ERR_NOSUCHNICK(c.getNickname(), nickname) << std::endl;
        return;
    }

    Client client = s.getClient(nickname);
    if (client.getNickname() != nickname)
    {
        std::cout << ERR_NOSUCHNICK(c.getNickname(), nickname) << std::endl;
        return;
    }

    userInChannel = channel.getClientInChannel(nickname);
    if (userInChannel.getNickname() != nickname)
    {
        std::cout << ERR_USERNOTINCHANNEL(c.getNickname(), channelName) << std::endl;
        return;
    }

    handleMode(mode, channel, c, userInChannel);
}