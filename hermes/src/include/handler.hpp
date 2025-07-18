#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/data/ia.hpp"
#include "../../data_lcm/data/tartarus.hpp"
#include "../../build/grSim_Packet.pb.h"

class handler {
    public:
        void handleIA(const lcm::ReceiveBuffer* rbuf,
                           const std::string& chan, 
                           const data::ia* data_ia);
    
        void handletartarus(const lcm::ReceiveBuffer* rbuf,
                           const std::string& chan, 
                           const data::tartarus * data_tartarus);

        data::ia data_ia_copy;
        data::tartarus data_tartarus_copy;
    };

extern handler han;

#endif // HANDLER_HPP