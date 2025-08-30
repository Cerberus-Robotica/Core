#include "include/tartarus.hpp"
#include "include/socket_connect.hpp"
#include <lcm/lcm-cpp.hpp>

Handler han;

void Handler::handle_tartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan,
    const data::tartarus* msg_tartarus) {
        
    this->new_tartarus = *msg_tartarus;

    updated_tartarus = !updated_tartarus;
}