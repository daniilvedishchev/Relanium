#pragma once

#include <stdexcept>

#include <mutex>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <unordered_set>

#include <thread>

#include "socket/status/status.hpp"
#include "errors/AcceptException.hpp"
#include "errors/SocketException.hpp"
#include "errors/ListeningException.hpp"
#include "errors/BindException.hpp"
#include "errors/SendException.hpp"

class Socket {
private:
    int fd = -1;

    Status status;

    uint16_t port;
    uint16_t backlog;
    
    std::thread waiter;

    std::unordered_set<int> clients;

    sockaddr_in address;
    
    std::mutex clientMutex;

    void makeIPv4TCPAddress();
    void attachAddressToOpenSocket();
    void waitForClients();
public:
    Socket(uint16_t port,uint16_t backlog);
    void sendSignal(std::string& signal);
};
