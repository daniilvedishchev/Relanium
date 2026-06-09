#include "src/relanium/relanium.hpp"
#include <chrono>
#include <string>
#include <thread>

/**
 * @brief signal := "SYMBOL SIDE QTY SL TP"
*/

relanium::relanium(uint16_t port, uint16_t backlog) : socket_(Socket(port,backlog)){}

void relanium::send(std::string& signal){
    socket_.sendSignal(signal);
}

int main(){
    relanium RELANIUM = relanium(8080,10);
    while (true) {
        std::string signal = "BTCUSD BUY 0.01 45000 75000\n";
        RELANIUM.send(signal);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
