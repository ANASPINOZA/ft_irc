/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkIfDeconected.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 15:43:41 by ahel-mou          #+#    #+#             */
/*   Updated: 2023/08/21 11:18:29 by ahel-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Commands.hpp"

void checkIfDisconnected(Server &server)
{
    std::map<std::string, Channel> channels = server.getChannels();

    for (std::map<std::string, Channel>::iterator channelIt = channels.begin(); channelIt != channels.end(); ++channelIt)
    {
        Channel &channel = channelIt->second;
        std::map<std::string, Client> clients = channel.getChannelClients();

        for (std::map<std::string, Client>::iterator clientIt = clients.begin(); clientIt != clients.end();)
        {
            const std::string &nickname = clientIt->second.getNickname();
            bool clientDisconnected = !server.isNickThere(server, nickname);

            if (clientDisconnected)
            {
                if (channel.getChannelOwner() == nickname)
                {
                    std::map<std::string, Client>::iterator nextClientIt = clients.begin();
                    if (nextClientIt != clientIt)
                    {
                        channel.setChannelOwner(nextClientIt->second.getNickname());
                    }
                    else
                    {
                        channel.setChannelOwner("");
                    }
                }
                channel.removeClientFromChannel(server, clientIt->second, channelIt->first);
                std::map<std::string, Client>::iterator tempIt = clientIt;
                ++clientIt;
                clients.erase(tempIt);
            }
            else
            {
                ++clientIt;
            }
        }
    }
}
