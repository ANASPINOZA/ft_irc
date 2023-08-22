#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../server.hpp"
#include "../client.hpp"
#include "../Channel.hpp"

class Client;
class Server;
class Channel;

#define ERR_USERONCHANNEL(nick, channel) "443 " + nick + " " + channel + " :is already on channel"
#define ERR_USERNOTINCHANNEL(nick, channel) "441 " + nick + " " + channel + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"
#define ERR_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"
#define ERR_NEEDMOREPARAMS(nick) "461 " + nick + " :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"
#define ERR_NOSUCHNICK(nick, channel) "401 " + nick + " " + channel + " :No such nick"
#define ERR_UNKNOWNMODE(nick, mode) "472 " + nick + " " + mode + " :is unknown mode char to me"
#define ERR_KEYSET(nick, channel) "467 " + nick + " " + channel + " :Channel key already set"
#define ERR_CHANNELISFULL(nick, channel) "471 " + nick + " " + channel + " :Cannot join channel (+l)"
#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)"
#define RPL_CHANNELMODEIS(nick, channel, mode) "324 " + nick + " " + channel + " " + mode + " :Channel modes"
#define RPL_UMODEIS(nick, mode) "221 " + nick + " " + mode + " :is your mode"
#define RPL_KICK(nick, target) "381 " + nick + target + " :Has been kicked"
#define RPL_INVITING(nick, user, channel) "341 " + nick + " " + user + " " + channel + " :Inviting"
#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " =" + topic
#define RPL_NOTOPIC(nick, channel) "331 " + nick + " " + channel + " :No topic is set"
#define RPL_YOUREOPER(nick) "381 " + nick + " :You are now an IRC operator"
#define ERR_BOT(nick) "381 " + nick + " :Bot Option is empty Usage: BOT :<option>  Options: -d for dad jokes -s for random song -g for guess -f to get interesting facts" + "\r\n"
#define RPL_BOT(nick) "381 " + nick
#define ERR_CANTKICK(nick) "482 " + nick + " :You can't kick yourself"
#define ERR_NICKNAMEINUSE(nick) "433 " + nick + " :Nickname is already in use"
#define ERR_NONICKNAMEGIVEN "431" \
                            " :No nickname given\r\n"
#define ERR_BADTOPIC(nick, channel) "442 " + nick + " " + channel + " :Bad Topic"
class commands
{
public:
    void Kick(Client &c, Server &s);
    void Topic(Client &c, Server &s);
    void Invite(Client &c, Server &s);
    void Mode(Client &c, Server &s);
    void Join(Client &client, Server &server);
    void Privmsg(Client &client, Server &server);
    void Bot(Client &client, Server &server);
};

std::vector<std::string> splitCommand(const std::string &cmd);
std::string parseModeOptions(const std::string &input);
std::string getTextAfterColon(const std::string &cmd);
void sendMessage(const std::string &message, int fd);
void sendMessageToChannel(Channel &channel, const std::string &message);
void checkIfDisconnected(Server &server);
#endif
