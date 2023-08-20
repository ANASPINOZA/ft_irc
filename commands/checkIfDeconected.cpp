/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkIfDeconected.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 15:43:41 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/20 15:53:29 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

// check if deconnected and remove from the channels he is in

void checkIfDisconnected(Server &server)
{
    std::map<std::string, Channel>::iterator channelIt = server.getChannels().begin();

    while (channelIt != server.getChannels().end())
    {
        std::map<std::string, Client> clients = channelIt->second.getChannelClients();
        std::map<std::string, Client>::iterator clientIt = clients.begin();

        while (clientIt != clients.end())
        {
            if (!server.isNickThere(server, clientIt->second.getNickname()))
            {
                channelIt->second.removeClientFromChannel(server, clientIt->second, channelIt->first);
                clientIt = clients.erase(clientIt);
            }
            else
            {
                ++clientIt;
            }
        }

        ++channelIt;
    }
}