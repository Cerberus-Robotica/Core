#ifndef SENDO_TO_ROBOTS_HPP
#define SENDO_TO_ROBOTS_HPP

#include "handler.hpp"
#include "../../build/grSim_Packet.pb.h"
#include "socket_connect.hpp"
#include "stm.hpp"
#include "manual_control.hpp"

class robots_sender {
    public:
        void send_to_grsim();
        void send_control();

        grSim_Commands commands;
        grSim_Packet packet_grsim;

        bool updated = false;
};

extern robots_sender sender;

#endif // SENDO_TO_GRSIM_HPPs