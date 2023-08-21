/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:17:49 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/21 00:48:01 by ielmakhf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include "User.hpp"
#include <vector>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include "server.hpp"

class Server;
class commands;

#define IS_OP 4
#define IS_NOT_OP 5

class Server;

class Client
{
private:
    std::string nickname;
    // User            user;
    char *ipv4;
    int fd;
    std::string username;
    std::string usermode;
    std::string unused;
    std::string realname;
    int isOperator;
    bool    pass;
    bool    nick;
    bool    user;
    bool    Authen;

public:
    Client();
    ~Client();
    std::vector<std::string> tokens;
    std::vector<std::string> getTokens();
    void addData(std::string &new_token);
    void addVector(Server &s, std::vector<std::string> new_vector, int fds_fd);
    void printData();
    void setIPV4(char *ip);
    void setNickname(std::string nickname);
    void setUser(std::string username, std::string hostname, std::string realname);
    void setUserName(std::string username);
    void setUserMode(std::string usermode);
    void setUnused(std::string unused);
    void setRealName(std::string realname);
    std::string getUserName();
    std::string getUserMode();
    std::string getUnused();
    std::string getRealName();
    std::string getNickname();
    int getOp();
    void setPass(bool type);
	void setNick(bool type);
	void setUser(bool type);
    void setAuthen(bool type);
    bool getPass();
    bool getUser();
    bool getNick();
    bool getAuthen();
    // void            parser(std::vector<std::string> tokens);
    // void            checkTokens(std::string cmd, std::string param, int *i);
    // void            checkTokens(std::vector<std::string> cmd);
    // int            checkCmds(std::string cmd);
    // void            Join(std::string param);
    // void            Join(Client &client ,Server &server , int i);
    // void            checkInviteParam(std::string param);
    // void            checkKickParam(std::string param);
    // void            checkTopicParam(std::string param);
    // void            Privmsg(std::string param);
    void setOP(int state);
    // void            delelet
    int getFd() const;
    void setFd(Server &s, int Fd);
    // User            getUser();
};

std::string getHostName();
std::vector<std::string> splitStrToVec(std::string str, char del);
std::vector<std::string> splitVec(std::vector<std::string> vec, char del);

#endif