/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkIfDeconected.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 15:43:41 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/20 15:57:20 by ahel-mou         ###   ########.fr       */
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
                if (channelIt->second.getChannelOwner() == clientIt->second.getNickname())
                {
                    // set the next client as the owner
                    std::map<std::string, Client>::iterator nextClientIt = clients.begin();
                    nextClientIt++;
                    if (nextClientIt != clients.end())
                        channelIt->second.setChannelOwner(nextClientIt->second.getNickname());
                    else
                        channelIt->second.setChannelOwner("");
                }
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