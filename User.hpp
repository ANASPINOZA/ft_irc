/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aadnane <aadnane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 19:21:52 by aadnane           #+#    #+#             */
/*   Updated: 2023/07/14 19:46:11 by aadnane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include <iostream>
#include <string>

class   User
{
    private:
        std::string username;
        std::string hostname;
        std::string realname;
    public:
        User();
        ~User();
        void    setUsername(std::string username);
        void    setHostname(std::string hostname);
        void    setRealname(std::string realname);
        std::string     getUsername();
        std::string     getHostname();
        std::string     getRealname();
};

#endif //USER_HPP