/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahel-mou <ahmed@1337.ma>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 19:17:49 by aadnane           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/07/31 19:51:07 by ahel-mou         ###   ########.fr       */
=======
/*   Updated: 2023/08/01 16:19:55 by aadnane          ###   ########.fr       */
>>>>>>> dev
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include "User.hpp"

class Client
{
private:
    std::string PASS;
    std::string nickname;
    User user;
    char *ipv4;

public:
    Client();
    ~Client();
    void setPASS(std::string password);
    void setIPV4(char *ip);
    void setNickname(std::string nickname);
    void setUser(std::string username, std::string hostname, std::string realname);
    std::string getNickname();
    std::string getPassword();
    User getUser();
};

#endif