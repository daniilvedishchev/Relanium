#include "src/relanium/relanium.hpp"

/**
 * @brief signal := "SYMBOL SIDE QTY SL TP"
*/

relanium::relanium(uint16_t port, uint16_t backlog) : socket_(Socket(port,backlog)){}

void relanium::send(std::string& signal){
    socket_.sendSignal(signal);
}

int main(){
    Socket RELANIUM(8080,10);
    while (true) {}
    return 0;
}
