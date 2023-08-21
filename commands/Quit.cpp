/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 11:56:14 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 16:11:43 by ielmakhf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void commands::Quit(Client &c, Server &s, int idx)
{
    int fd;
    std::vector<std::string> cmd = splitCommand(c.getTokens()[1]);
    std::string quitMsg;
    if (cmd.size() >= 1)
        quitMsg = getTextAfterColon(c.getTokens()[1]);

    if (quitMsg.empty())
        quitMsg = "Client Quit";
    if (s.isNickThere(s, c.getNickname()))
    {
        fd = c.getFd();
        if (s.removeClientFromServer(s, c.getFd(), idx))
        {
            sendMessage(c.getNickname() + " has quit (" + quitMsg + ")\r\n", fd);
            close(fd);
        }
        else
            sendMessage("ERROR: Failed to quit\r\n", c.getFd());
    }
}

void commands::Ping(Client &c)
{
    sendMessage(c.getNickname() + " 406" + " :PING" + "\r\n", c.getFd());
}