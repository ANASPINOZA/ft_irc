/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ielmakhf <ielmakhf@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:17:49 by aadnane           #+#    #+#             */
/*   Updated: 2023/07/22 15:34:15 by ielmakhf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include "User.hpp"

class   Client
{
    private:
        std::string     PASS;
        std::string     nickname;
        User            user;
        char            *ipv4;
    public:
        Client();
        ~Client();
        void    setPASS(std::string password);
        void    setIPV4(char    *ip);
        void    setNickname(std::string nickname);
        void    setUser(std::string username, std::string hostname, std::string realname);
        std::string     getNickname();
        std::string     getPassword();
        User            getUser();
};


#endif //CLIENT_HPP