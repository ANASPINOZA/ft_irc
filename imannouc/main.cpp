#include <iostream>
#include <signal.h>
#include "srcs/server/server.hpp"

void shutdown(int signal) {
    if (signal == SIGINT) {
        std::cout << " received SIGINT.." << std::endl;
    }
}

int main(int ac, char **av) {

    try {
        if (ac == 2) {
            MainServer server;
            parse_config(av[1], server);
            server.init_server();

            signal(SIGINT, &shutdown);
            server.run();
        }
        else
            throw std::runtime_error("Provide config file..");
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    
    
    return 0;
}