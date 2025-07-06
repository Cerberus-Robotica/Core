#include "handler.hpp"

handler han;

void handler::handleIA(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::ia * data_ia) {

    this->data_ia_copy = *data_ia;
    //std::cout << "timestamp" << " " << this->data_ia_copy.timestamp << std::endl;
    //std::cout << "robot size " << this->data_ia_copy.robots_size << std::endl;
    //std::cout << "timestamp " << this->data_ia_copy.timestamp << std::endl;
}

void handler::handletartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::tartarus * data_tartarus) {

    this->data_tartarus_copy = *data_tartarus;
    //std::cout << "Received tartarus message" << std::endl;
    // Handle tartarus message
    //std::cout << "estrategia " << data_tartarus->estrategia << std::endl;
    //std::cout << "processo " << data_tartarus->processo << std::endl;
    //std::cout << "ssl_vision " << data_tartarus->ssl_vision << std::endl;
}