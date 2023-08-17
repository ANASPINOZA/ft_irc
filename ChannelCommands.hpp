#ifndef CHANNELCOMMANDS_HPP
# define CHANNELCOMMANDS_HPP

# include <iostream>
# include <string>
#include <map>
#include <vector>
#include "client.hpp"
#include "server.hpp"

class Client;
class Server;

// class ChannelCommands
// {

// 	public:

// };
void            checkJoinParam(Client &client ,Server &server);
void            checkInviteParam(std::string param);
void            checkKickParam(std::string param);
void            checkTopicParam(std::string param);
// void            checkPrivmsgParam(std::string param);
void            checkPrivmsgParam(Client &client ,Server &server);
void            checkTokens(std::vector<std::string> cmd);
int				checkCmds(std::string cmd);
void            parser(std::vector<std::string> tokens);

// std::ostream &			operator<<( std::ostream & o, ChannelCommands const & i );

#endif /* ************************************************* CHANNELCOMMANDS_H */

// void	IrcServer::check_Privmsg_cmd(const std::vector<std::string> &command, Client_irc *client)
// {
// 	if (command.size() < 3)
//     {
//         client->msg = ":" + getMachineHost() + " 461 " + client->get_nick() + " :PRIVMSG command requires 2 arguments\r\n";
//         client->send_msg_to_client();
//     }
//     else
//     {
//         std::vector<std::string> receivers = split_string(command[1], ',');
//         for (size_t i = 0; i < receivers.size(); i++)
//         {
//             if (receivers[i][0] == '#')
//             {
//                 if (mapchannels.find(receivers[i]) == mapchannels.end())
//                 {
//                     client->msg = ":" + getMachineHost() + " 401 " + client->get_nick() + " " + command[1] + " :channel doesn't exist\r\n";
//                     client->send_msg_to_client();
//                 }
//                 else
//                 {
//                     if (mapchannels[receivers[i]].clients.find(client->get_nick()) == mapchannels[receivers[i]].clients.end())
//                     {
//                         client->msg = ":" + getMachineHost() + " 401 " + client->get_nick() + " " + command[1] + " :you are not in this channel\r\n";
//                         client->send_msg_to_client();
//                     }
//                     else
//                     {
//                         std::string message = ":" + client->get_nick() + "!" + client->get_user().username + "@" + getMachineHost() + " PRIVMSG " + command[1] + " " + command[2];
//                     for (size_t j = 3; j < command.size(); j++)
//                     {
//                         message += " " + command[j] ;
//                     }
//                     message += "\r\n";
//                     mapchannels[receivers[i]].broadcast(message, client->fd_client);
//                     }
//                 }
//             }
//             else
//             {
//                 if (mapclients.find(client_finder(receivers[i])) == mapclients.end())
//                 {
//                     client->msg = ":" + getMachineHost() + " 401 " + client->get_nick() + " " + command[1] + " :user doesn't exist\r\n";
//                     client->send_msg_to_client();
//                 }
//                 else
//                 {
//                     std::string message = ":" + client->get_nick() + "!" + client->get_user().username + "@" + getMachineHost() + " PRIVMSG " + command[1] + " " + command[2] ;
//                     for (size_t j = 3; j < command.size(); j++)
//                     {
//                         message += " " + command[j] ;
//                     }
//                     message += "\r\n";
//                     mapclients[client_finder(receivers[i])].msg = message;
//                     mapclients[client_finder(receivers[i])].send_msg_to_client();
//                 }
//             }
//         }
//     }
// }