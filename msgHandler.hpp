#ifndef MSGHANDLER_HPP
#define MSGHANDLER_HPP
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <iostream>

void sendMessage(const std::string &message, int fd);

#endif
