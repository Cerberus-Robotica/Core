#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/ia.hpp"
#include "../../data_lcm/tartarus.hpp"
#include "../../data_lcm/game_controller.hpp"
#include "../../build/grSim_Packet.pb.h"

class handler {
    public:
        void handleIA(const lcm::ReceiveBuffer* rbuf,
                           const std::string& chan, 
                           const data::ia* data_ia);
    
        void handletartarus(const lcm::ReceiveBuffer* rbuf,
                           const std::string& chan, 
                           const data::tartarus * data_tartarus);

        void handleGC(const lcm::ReceiveBuffer* rbuf,
                           const std::string& chan, 
                           const data::game_controller* data_gc);

        data::ia data_ia_copy;
        data::tartarus data_tartarus_copy;
        data::game_controller data_gc_copy;
        bool updated_tartarus = false;
    };

extern handler han;

#endif // HANDLER_HPP