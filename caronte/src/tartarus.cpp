#include "include/tartarus.hpp"
#include <lcm/lcm-cpp.hpp>

data::tartarus my_tartarus_data;


void Handler::handle_tartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan,
    const data::tartarus* msg_tartarus) {

    system("clear");
    std::cout << "Received message on channel \"" << chan << "\"" << std::endl;
    std::cout << "   estrategia -> " << msg_tartarus->estrategia << "\n" << std::endl; // 0 para false, 1 para true
    std::cout << "   processo -> " << msg_tartarus->processo << "\n" << std::endl;
    std::cout << "   ssl_vision -> " << (int)msg_tartarus->ssl_vision << "\n" << std::endl; // 0 para false, 1 para true
}