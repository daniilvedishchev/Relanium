#pragma once 

#include "socket/socket.hpp"
#include <cstdint>
#include <string>
#include <chrono>
#include <thread>

class relanium {
private:
    Socket socket_;
public:
    relanium(uint16_t port, uint16_t backlog);
    void send(std::string& signal);
};
