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
    for (auto &client : channel.getChannelClients())
    {
        sendMessage(message, client.second.getFd());
    }
    sendMessage(message, sender.getFd());
}