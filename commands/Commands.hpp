#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "../server.hpp"
#include "../client.hpp"

class commands
{
    public:
        void Kick(std::vector<std::string> params);
        void Topic(std::vector<std::string> params);
        void Invite(std::vector<std::string> params);
        void Mode(std::vector<std::string> params);
};

#endif