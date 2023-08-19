/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:05 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/19 18:17:02 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void handleMode(const std::string &mode, Channel &channel, Client &c, Client &user, std::string &key)
{
    if (mode.empty() || mode.size() < 2)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    if (!channel.isOperator(c.getNickname()))
    {
        std::string errorMsg = ERR_CHANOPRIVSNEEDED(c.getNickname(), channel.getChannelName()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
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
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption) + "\r\n", c.getFd());
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyInvited(PUBLIC_CHANNEL);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) + "\r\n", c.getFd());
        }
        break;

    case 't':
        if (modeSymbol == '+')
        {
            channel.setOnlyOperatorTopic(true);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption) + "\r\n", c.getFd());
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyOperatorTopic(false);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) + "\r\n", c.getFd());
        }
        break;

    case 'o':
        if (modeSymbol == '+')
        {
            if (channel.isOperator(user.getNickname()))
            {
                sendMessage(ERR_USERONCHANNEL(c.getNickname(), user.getNickname()) + "\r\n", c.getFd());
            }
            else
            {
                channel.setChannelOperators(user.getNickname());
                sendMessage(RPL_UMODEIS(c.getNickname(), "+" + modeOption) + "\r\n", c.getFd());
            }
        }
        else if (modeSymbol == '-')
        {
            if (channel.isOperator(user.getNickname()))
            {
                channel.removeOperator(user.getNickname());
                sendMessage(RPL_UMODEIS(c.getNickname(), "-" + modeOption) + "\r\n", c.getFd());
            }
            else
            {
                sendMessage(ERR_USERNOTINCHANNEL(c.getNickname(), user.getNickname()) + "\r\n", c.getFd());
            }
            break;
        }

    case 'k':
        if (modeSymbol == '+')
        {
            channel.setProtectedByPassword(true);
            channel.setChannelPassword(key);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption) + "\r\n", c.getFd());
        }
        else if (modeSymbol == '-')
        {
            if (channel.getProtectedByPassword())
            {
                channel.setProtectedByPassword(false);
                channel.setChannelPassword("");
                sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) + "\r\n", c.getFd());
            }
        }
        else
        {
            sendMessage(ERR_KEYSET(c.getNickname(), channel.getChannelName()) + "\r\n", c.getFd());
        }
        break;

    case 'l':
        if (modeSymbol == '+')
        {
            channel.setMaxNumUsers(std::stoi(key));
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption + " " + key) + "\r\n", c.getFd());
        }
        else if (modeSymbol == '-')
        {
            channel.setMaxNumUsers(0);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) + "\r\n", c.getFd());
        }
        break;

    default:
        sendMessage(ERR_UNKNOWNMODE(c.getNickname(), modeOption) + "\r\n", c.getFd());
    }
}

void commands::Mode(Client &c, Server &s)
{
    std::vector<std::string> cmd = splitCommand(c.getTokens()[1]);
    if (cmd.size() < 2 || cmd.size() > 3)
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

    std::string channelName = cmd[0];
    std::string mode = parseModeOptions(cmd[1]);
    std::string target = cmd[2];

    if (mode.empty())
        return;

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
    if (mode[0] == 'o')
    {
        std::string nickname = target;
        Client client = s.getClient(s, nickname);
        if (client.getNickname() != nickname)
        {
            std::string errorMsg = ERR_NOSUCHNICK(c.getNickname(), nickname) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }

        if (!s.isNickInChannel(s, nickname, channelName))
        {
            std::string errorMsg = ERR_NOSUCHNICK(nickname, channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }

        userInChannel = channel.getClientInChannel(nickname);
        if (userInChannel.getNickname() != nickname)
        {
            std::string errorMsg = ERR_USERNOTINCHANNEL(c.getNickname(), channelName) + "\r\n";
            sendMessage(errorMsg, c.getFd());
            return;
        }
    }

    handleMode(mode, channel, c, userInChannel, target);
    // print channel password
    std::cout << "channel password : " << channel.getChannelPassword() << std::endl;
    // print channel topic
}
