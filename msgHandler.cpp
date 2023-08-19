#include "commands/Commands.hpp"

void sendMessage(const std::string &message, const int fd)
{
    if (send(fd, message.c_str(), message.length(), 0) == -1)
    {
        perror("send message error");
    }
}

void sendMessageToChannel(Channel &channel, const Client &sender, const std::string &message)
{
    const std::map<std::string, Client> &channelClients = channel.getChannelClients();
    std::map<std::string, Client>::const_iterator it;

    for (it = channelClients.begin(); it != channelClients.end(); ++it)
    {
        const Client &client = it->second;
        sendMessage(message, client.getFd());
    }

    sendMessage(message, sender.getFd());
}
