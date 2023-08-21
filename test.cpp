
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <vector>
#include <map>
#include <string>
#include <string.h>
#include "client.hpp"
#include <sys/poll.h>
#include <sstream>
#include <fcntl.h>


int main()
{
    char test[100];

    fcntl(1, F_SETFL, O_NONBLOCK);
    read(1, test, 100);

    std::cout << test << std::endl;

}