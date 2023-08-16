#ifndef MSGHANDLER_HPP
#define MSGHANDLER_HPP
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "Channel.hpp"
#include "User.hpp"
#include "server.hpp"
#include "client.hpp"


void sendMessage(const std::string &message, int fd);

#endif
