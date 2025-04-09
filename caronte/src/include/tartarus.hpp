#ifndef TARTARUS_HPP
#define TARTARUS_HPP

#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/data/tartarus.hpp"

extern data::tartarus my_tartarus_data;

class Handler {
    public:
    void handle_tartarus(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const data::tartarus* msg_tartarus) {
        //std::cout << "\n----------------------------------------\n";
        //std::cout << "            C E R B E R U S \n";
        //std::cout << "----------------------------------------" << std::endl;
        system("clear");
        std::cout << "Received message on channel \"" << chan << "\"" << std::endl;
        std::cout << "   estrategia -> " << msg_tartarus->estrategia << "\n" << std::endl; // 0 para false, 1 para true
        std::cout << "   processo -> " << msg_tartarus->processo << "\n" << std::endl;
        std::cout << "   ssl_vision -> " << (int)msg_tartarus->ssl_vision << "\n" << std::endl; // 0 para false, 1 para true
}
    lcm::LCM* lc;

};

#endif