#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/game_controller.hpp"
#include "../../data_lcm/vision.hpp"
#include "../../data_lcm/ia.hpp"
#include "../../data_lcm/tartarus.hpp"
#include "../../data_lcm/autoref.hpp"

class handlers {
    public:
        void handle_GC(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan,
                       const data::game_controller* msg_GC);
    
        void handle_ia_vision(const lcm::ReceiveBuffer* rbuf,
                           const std::string& chan,
                           const data::vision* msg_vision);
    
        void handle_tartarus(const lcm::ReceiveBuffer* rbuf,
                             const std::string& chan,
                             const data::tartarus* msg_tartarus);
        
        void handle_autoref(const lcm::ReceiveBuffer* rbuf,
                            const std::string& chan,
                            const data::autoref* msg_autoref);

        lcm::LCM* lc;
        data::ia new_ia;
        data::tartarus new_tartarus;
        data::game_controller new_GC;
        data::vision new_vision;
        data::autoref new_autoref;
    };

extern handlers han;

#endif