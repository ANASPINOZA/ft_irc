/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:17:49 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/11 18:04:21 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include "User.hpp"
#include <vector>
#include <algorithm>
#include "server.hpp"


class Client
{
    private:
        std::string     nickname;
        bool            pass;
        // User            user;
        char            *ipv4;
        int             fd;
        std::string username;
        std::string usermode;
        std::string unused;
        std::string realname;
    public:
        Client();
        ~Client();
        std::vector<std::string> tokens;
        std::vector<std::string> getTokens();
        void    setPass(bool type);
        void    addData(std::string &new_token);
        void    addVector(std::vector<std::string> new_vector);
        void    printData();
        void    setIPV4(char    *ip);
        void    setNickname(std::string nickname);
        void    setUser(std::string username, std::string hostname, std::string realname);
        void    setUserName(std::string username);
        void    setUserMode(std::string usermode);
        void    setUnused(std::string unused);
        void    setRealName(std::string realname);
        std::string     getUserName();
        std::string     getUserMode();
        std::string     getUnused();
        std::string     getRealName();
        std::string     getNickname();
        void            parser(std::vector<std::string> tokens);
        // void            checkTokens(std::string cmd, std::string param, int *i); 
        void            checkTokens(std::vector<std::string> cmd);
        int            checkCmds(std::string cmd);
        // void            checkJoinParam(std::string param);
        void            checkJoinParam(std::vector<std::string> cmd, int i);
        void            checkInviteParam(std::string param);
        void            checkKickParam(std::string param);
        void            checkTopicParam(std::string param);
        void            checkPrivmsgParam(std::string param);
        // void            delelet
        // std::string     getFd();
        bool     getPass();
        // User            getUser();
};

#endif