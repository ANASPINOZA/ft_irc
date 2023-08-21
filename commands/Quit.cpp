/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 11:56:14 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 12:15:33 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void commands::Quit(Client &c, Server &s)
{
    std::vector<std::string> cmd = splitCommand(c.getTokens()[1]);
    std::string quitMsg;
    if (cmd.size() >= 1)
        quitMsg = getTextAfterColon(c.getTokens()[1]);

    if (quitMsg.empty())
        quitMsg = "Client Quit";
    if (s.isNickThere(s, c.getNickname()))
    {
        if (s.removeClientFromServer(s, c.getFd()))
            sendMessage(c.getNickname() + " has quit (" + quitMsg + ")\r\n", c.getFd());
        else
            sendMessage("ERROR: Failed to quit\r\n", c.getFd());
    }
}