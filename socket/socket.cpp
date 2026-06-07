#include "socket/socket.hpp"

Socket::Socket(uint16_t port, uint16_t backlog) : address{}, status(Status::Running), port(port), backlog(backlog) {
    fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (fd == -1){
        throw SocketException("Socket creation failed.");
    }

    makeIPv4TCPAddress();
    attachAddressToOpenSocket();

    int result = listen(fd,backlog);

    if (result == -1){
        throw ListeningException("Listen failed.");
    }

    waiter = std::thread([this](){
            waitForClients();
        }
    );
}

void Socket::makeIPv4TCPAddress(){
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

void Socket::waitForClients(){
    while (status == Status::Running){
        int client = accept(fd,nullptr,nullptr);

        if (client == -1) {
            continue;
        }
        
        {
            std::lock_guard<std::mutex> lock(clientMutex);
            clients.insert(client);
        }
    }
}

void Socket::attachAddressToOpenSocket(){
    int success = bind(
        fd,
        reinterpret_cast<sockaddr*>(&address),
        sizeof(address)
    );

    if (success == -1){
        throw BindException("Unsuccessful bind. Veriry if entered port is free.");
    }
}

void Socket::sendSignal(std::string& signal){
    std::unordered_set<int> clientCpy;
    
    {
        std::lock_guard<std::mutex> lock(clientMutex);
        clientCpy = clients;
    }

    for (auto& client : clientCpy){
        int success = send(client,signal.data(),signal.size(),0);
        if (success == -1) {
            throw SendException("Send failure.");
        }
    }
}