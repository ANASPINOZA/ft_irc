#include "msgHandler.hpp"


void sendMessage(const std::string& message, int fd) {
    if (send(fd, message.c_str(), message.length(), 0) == -1) {
        perror("send message error");
    }
}
