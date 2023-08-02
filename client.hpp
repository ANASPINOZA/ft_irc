/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:17:49 by aadnane           #+#    #+#             */
/*   Updated: 2023/08/02 18:19:50 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include "User.hpp"
#include <vector>

class Client
{
    private:
        std::string     nickname;
        bool            pass;
        User            user;
        char            *ipv4;
        int             fd;


    public:
        Client();
        ~Client();
        std::vector<std::string> tokens;
        std::vector<std::string> getTokens();
        void    setPass(bool type);
        void    addData(std::string &new_token);
        void    printData();
        void    setIPV4(char    *ip);
        void    setNickname(std::string nickname);
        void    setUser(std::string username, std::string hostname, std::string realname);
        std::string     getNickname();
        void            parser(std::vector<std::string> tokens);
        // std::string     getFd();
        bool     getPass();
        // User            getUser();
};

#endif