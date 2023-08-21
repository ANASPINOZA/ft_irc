#include "server.hpp"

int main(int ac, char **av)
{
    Server server;

    try
    {
        if (ac != 3)
            throw std::runtime_error("UTILS: ./IrcServer <PORT> <PASSWORD>");
        server.SomeParss(av);
        server.ft_server();
    }
    catch (std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}