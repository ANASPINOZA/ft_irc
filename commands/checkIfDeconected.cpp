/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkIfDeconected.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 15:43:41 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 11:55:31 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void checkIfDisconnected(Server &server)
{
    std::map<std::string, Channel> channels = server.getChannels();
    std::map<std::string, Channel>::iterator itChan = channels.begin();
    while (itChan != channels.end())
    {
        std::map<std::string, Client> clients = itChan->second.getChannelClients();
        std::map<std::string, Client>::iterator clientIt = clients.begin();
        while (clientIt != clients.end())
        {
            if (!server.isNickThere(server, clientIt->second.getNickname()))
            {
                if (itChan->second.isOperator(clientIt->second.getNickname()))
                    itChan->second.removeOperator(clientIt->second.getNickname());
                if (itChan->second.getChannelOwner() == clientIt->second.getNickname())
                    itChan->second.setChannelOwner("");
                itChan->second.removeClientFromChannel(server, clientIt->second, itChan->second.getChannelName());
            }
            clientIt++;
        }
        itChan++;
    }
}
