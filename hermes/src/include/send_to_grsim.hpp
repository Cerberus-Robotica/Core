#ifndef SENDO_TO_GRSIM_HPP
#define SENDO_TO_GRSIM_HPP

#include "handler.hpp"
#include "../../build/grSim_Packet.pb.h"
#include "socket_connect.hpp"


class grsim_sender {
    public:
        void send_to_grsim();

        grSim_Commands commands;
        grSim_Packet packet_grsim;

};

extern grsim_sender grsim_send;
//extern grSim_Packet packet_grsim;
//extern grSim_Commands commands;

#endif // SENDO_TO_GRSIM_HPPs