#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../server.hpp"
#include "../client.hpp"
#include "../channel.hpp"

class commands
{
public:
    void Kick(std::vector<std::string> cmd, Client c);
    void Topic(std::vector<std::string> cmd, Client c);
    void Invite(std::vector<std::string> cmd, Client c);
    void Mode(std::vector<std::string> cmd, Client c);
};

#endif