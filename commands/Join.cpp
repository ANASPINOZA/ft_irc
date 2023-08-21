#include "Commands.hpp"

void commands::Join(Client &client, Server &server)
{
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    size_t chanelsNum;
    size_t keysNum;
    std::string message;
    size_t userNum;
    std::vector<std::string> cmd;
    int i = 1;

    cmd = client.getTokens();
    cmd = splitVec(cmd, ' ');
    if (cmd.size() > 1)
    {
        if (cmd.size() > 2)
        {
            channels = splitStrToVec(cmd[i], ',');
            keys = splitStrToVec(cmd[i + 1], ',');
        }
        else
        {
            channels = splitStrToVec(cmd[i], ',');
            keys.push_back("");
        }
        chanelsNum = channels.size();
        keysNum = keys.size();
        if (chanelsNum >= keysNum)
        {
            for (size_t i = 0; i < channels.size(); i++)
            {
                int isChannelThere = server.isChannelIsThere(channels[i]);
                if (!isChannelThere)
                {
                    if (channels[i][0] == '#' && channels[i][1] != '\0')
                    {
                        Channel newChannel(channels[i], client);
                        newChannel.channelClients[client.getNickname()].setOP(IS_OP);
                        server.channel.insert(std::make_pair(channels[i], newChannel));
                        server.channel[channels[i]].channelOperators.push_back(client.getNickname());
                        userNum = server.channel[channels[i]].getUsersNum();
                        server.channel[channels[i]].setUsersNum(userNum + 1);
                        server.channel[channels[i]].setChannelPassword(keys[i]);
                        server.channel[channels[i]].setProtectedByPassword(true); //mountassir

                        message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                        sendMessage(message, client.getFd());
                        message = std::string(":") + getHostName() + " MODE " + channels[i] + " " + "\r\n";
                        sendMessage(message, client.getFd());
                        message = ":" + getHostName() + " 353 " + client.getNickname() + " = " + channels[i] + " :@" + client.getNickname() + "\r\n";
                        sendMessage(message, client.getFd());
                        message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + " :End of /NAMES list.\r\n";
                        sendMessage(message, client.getFd());
                    }
                    else
                    {
                        message = ":" + getHostName() + " 403 " + client.getNickname() + " :" + channels[i] + " No such channel\r\n";
                        sendMessage(message, client.getFd());
                        return;
                    }
                }
                else
                {

                    if (server.channel[channels[i]].getMaxNumUsers() > server.channel[channels[i]].getUsersNum() || server.channel[channels[i]].getMaxNumUsers() == 0)
                    {
                        if (server.channel[channels[i]].getOnlyInvited() == PRIVATE_CHANNEL)
                        {
                            // if channel is private (invitation only)
                            bool isInvited = server.channel[channels[i]].isClientisInvited(client.getNickname(), server);
                            if (!isInvited)
                            {
                                message = ":" + getHostName() + " 473 " + client.getNickname() + " :" + channels[i] + " Cannot join channel (+i)\r\n";
                                sendMessage(message, client.getFd());
                            }
                            else
                            {
                                // is invited code here
                                if (server.channel[channels[i]].channelClients.find(client.getNickname()) != server.channel[channels[i]].channelClients.end())
                                {
                                    message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
                                    sendMessage(message, client.getFd());
                                }
                                else
                                {
                                    server.channel[channels[i]].channelClients.insert(std::make_pair(client.getNickname(), client));
                                    userNum = server.channel[channels[i]].getUsersNum();
                                    server.channel[channels[i]].setUsersNum(userNum + 1);
                                    message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                                    server.channel[channels[i]].sendMsgToChannel(message, client.getFd());
                                    message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.channel[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
                                    sendMessage(message, client.getFd());
                                    message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
                                    sendMessage(message, client.getFd());
                                }
                            }
                        }
                        else
                        {
                            if (keys.size() > i && server.channel[channels[i]].getChannelPassword() != keys[i])
                            {
                                message = ":" + getHostName() + " 464 * :Password incorrect\r\n";
                                sendMessage(message, client.getFd());
                            }
                            else
                            {
                                std::map<std::string, Client> clients = server.channel[channels[i]].channelClients;
                                if (clients.find(client.getNickname()) != clients.end())
                                {
                                    message = ":" + getHostName() + " 400 " + client.getNickname() + " :this client is already exist in this channel !\r\n";
                                    sendMessage(message, client.getFd());
                                }
                                else
                                {
                                    server.channel[channels[i]].channelClients.insert(std::make_pair(client.getNickname(), client));
                                    userNum = server.channel[channels[i]].getUsersNum();
                                    server.channel[channels[i]].setUsersNum(userNum + 1);
                                    message = ":" + client.getNickname() + "!" + client.getUserName() + "@" + getHostName() + " JOIN " + channels[i] + "\r\n";
                                    server.channel[channels[i]].sendMsgToChannel(message, client.getFd());
                                    message = ":" + getHostName() + " 353 " + client.getNickname() + " @ " + channels[i] + " " + server.channel[channels[i]].getChannelMembers(channels[i], server) + "\r\n";
                                    sendMessage(message, client.getFd());
                                    message = ":" + getHostName() + " 366 " + client.getNickname() + " " + channels[i] + ":End of /NAMES list.\r\n";
                                    sendMessage(message, client.getFd());
                                }
                            }
                        }
                    }
                    else
                    {
                        sendMessage(ERR_CHANNELISFULL(client.getNickname(), channels[i]), client.getFd());
                        return;
                    }
                }
            }
        }
        else
        {
            message = ":" + getHostName() + " 400 " + client.getNickname() + "too much passwords for a channel\r\n";
            sendMessage(message, client.getFd());
        }
    }
    else
    {
        message = ":" + getHostName() + " 461 * " + " : " + "JOIN" + " Not enough parameters\r\n";
        sendMessage(message, client.getFd());
        return;
    }
}