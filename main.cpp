#include "server.hpp"


int main(void)
{
    Server  server;

    try {
        server.ft_server();
    }
    catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}