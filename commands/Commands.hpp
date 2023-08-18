#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../server.hpp"
#include "../client.hpp"
#include "../msgHandler.hpp"
#include "parsing.cpp"

class Client;
class Server;


// // -------------------------------------- MSGS -------------------------------------- //
#define ERR_USERONCHANNEL(nick, channel) "443 " + nick + " " + channel + " :is already on channel"
#define ERR_USERNOTINCHANNEL(nick, channel) "441 " + nick + " " + channel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"
#define ERR_NEEDMOREPARAMS(nick) "461 " + nick + " :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"
#define ERR_NOSUCHNICK(nick, channel) "401 " + nick + " " + channel + " :No such nick/channel"
#define ERR_UNKNOWNMODE(nick, mode) "472 " + nick + " " + mode + " :is unknown mode char to me"
#define ERR_KEYSET(nick, channel) "467 " + nick + " " + channel + " :Channel key already set"
#define ERR_CHANNELISFULL(nick, channel) "471 " + nick + " " + channel + " :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)"
#define RPL_CHANNELMODEIS(nick, channel, mode) "324 " + nick + " " + channel + " " + mode + " :Channel modes"
#define RPL_UMODEIS(nick, mode) "221 " + nick + " " + mode + " :is your mode"
#define RPL_KICK(nick, channel, user, reason) "KICK " + channel + " " + user + " " + reason
#define RPL_INVITING(nick, user, channel) "341 " + nick + " " + user + " " + channel + " :Inviting"
#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic
#define RPL_NOTOPIC(nick, channel) "331 " + nick + " " + channel + " :No topic is set"

class commands
{
    public:
        void Kick(Client &c, Server &s);
        void Topic(Client &c, Server &s);
        void Invite(Client &c, Server &s);
        void Mode(Client &c, Server &s);
        void            checkJoinParam(Client &client ,Server &server);

};

#endif

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
