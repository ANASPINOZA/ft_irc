/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:17:49 by aadnane           #+#    #+#             */
/*   Updated: 2023/07/14 19:46:13 by aadnane          ###   ########.fr       */
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
        std::string     password;
        std::string     nickname;
        User            user;
    public:
        Client();
        ~Client();
        void    setPassword(std::string password);
        void    setNickname(std::string nickname);
        void    setUser(std::string username, std::string hostname, std::string realname);
        std::string     getNickname();
        std::string     getPassword();
        User            getUser();
};


#endif //CLIENT_HPP