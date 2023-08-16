#include "msgHandler.hpp"

void sendMessage(const std::string &message, const int fd)
{
    if (send(fd, message.c_str(), message.length(), 0) == -1)
    {
        perror("send message error");
    }
}

void sendMessageToChannel(const Channel &channel, Client &sender, const std::string &message)
{
    const std::map<int, Client> &channelClients = channel.getChannelClients();
    std::map<int, Client>::const_iterator it;

    for (it = channelClients.begin(); it != channelClients.end(); ++it)
    {
        const Client &client = it->second;
        sendMessage(message, client.getFd());
    }

    sendMessage(message, sender.getFd());
}