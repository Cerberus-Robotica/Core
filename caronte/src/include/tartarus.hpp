#ifndef TARTARUS_HPP
#define TARTARUS_HPP

#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/tartarus.hpp"

class Handler {
    public:
    void handle_tartarus(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const data::tartarus* msg_tartarus);

    lcm::LCM* lc;
    data::tartarus new_tartarus;
    bool updated_tartarus = false;
};

extern Handler han;

#endif