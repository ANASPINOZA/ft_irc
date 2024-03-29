/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 22:28:05 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 21:08:24 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void handleMode(const std::string &mode, Channel &channel, Client &c, std::string &key)
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
        else
            sendMessage(ERR_UNKNOWNMODE(c.getNickname(), modeOption) + "\r\n", c.getFd());
        break;

    case 't':
        if (modeSymbol == '+')
        {
            channel.setOnlyOperatorTopic(true);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption) + "\r\n", c.getFd());
            sendMessage(RPL_TOPIC(c.getNickname(), channel.getChannelName(), channel.getChannelTopic()) + "\r\n", c.getFd());
        }
        else if (modeSymbol == '-')
        {
            channel.setOnlyOperatorTopic(false);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) + "\r\n", c.getFd());
        }
        else
            sendMessage(ERR_UNKNOWNMODE(c.getNickname(), modeOption) + "\r\n", c.getFd());
        break;
    case 'k':
        if (modeSymbol == '+')
        {
            if (key.empty())
            {
                sendMessage(ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n", c.getFd());
                return;
            }
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
            else
                sendMessage("000 ERROR: Channel is not protected by password\r\n", c.getFd());
        }
        else
            sendMessage(ERR_UNKNOWNMODE(c.getNickname(), modeOption) + "\r\n", c.getFd());
        break;

    case 'l':
        if (modeSymbol == '+')
        {
            if (key.empty())
            {
                sendMessage(ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n", c.getFd());
                return;
            }
            try
            {
                int numUsers = std::stoi(key);
                if (numUsers > std::numeric_limits<int>::max())
                {
                    sendMessage("000 ERROR: Value is too large\r\n", c.getFd());
                    return;
                }
                if (numUsers > 0)
                {
                    channel.setMaxNumUsers(numUsers);
                    sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "+" + modeOption + " " + key) + "\r\n", c.getFd());
                }
                else
                    sendMessage("000 ERROR: Value must be greater than 0\r\n", c.getFd());
            }
            catch (const std::invalid_argument &)
            {
                sendMessage("000 ERROR: Invalid number format\r\n", c.getFd());
            }
            catch (const std::out_of_range &)
            {
                sendMessage("000 ERROR: Value is out of range for int\r\n", c.getFd());
            }
        }
        else if (modeSymbol == '-')
        {
            channel.setMaxNumUsers(0);
            sendMessage(RPL_CHANNELMODEIS(c.getNickname(), channel.getChannelName(), "-" + modeOption) + "\r\n", c.getFd());
        }
        else
            sendMessage(ERR_UNKNOWNMODE(c.getNickname(), modeOption) + "\r\n", c.getFd());
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
    {
        std::string errorMsg = ERR_NEEDMOREPARAMS(c.getNickname()) + "\r\n";
        sendMessage(errorMsg, c.getFd());
        return;
    }

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
    if (mode[1] == 'o')
    {
        std::string nickname = target;
        if (nickname.empty())
        {
            sendMessage(ERR_NOSUCHNICK(c.getNickname(), nickname) + "\r\n", c.getFd());
            return;
        }
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

        if (mode[0] == '+')
        {
            if (channel.isOperator(userInChannel.getNickname()))
                sendMessage("000 ERROR: User is already an operator\r\n", c.getFd());
            else
            {
                channel.setChannelOperators(userInChannel.getNickname());
                sendMessage(RPL_YOUREOPER(userInChannel.getNickname()) + "\r\n", userInChannel.getFd());
            }
        }
        else if (mode[0] == '-')
        {
            if (channel.isOperator(userInChannel.getNickname()))
            {
                channel.removeOperator(userInChannel.getNickname());
                sendMessage(RPL_UMODEIS(c.getNickname(), "-" + mode[1]) + "\r\n", c.getFd());
            }
            else
                sendMessage(ERR_USERNOTINCHANNEL(c.getNickname(), userInChannel.getNickname()) + "\r\n", c.getFd());
        }
        return;
    }

    handleMode(mode, channel, c, target);
}
