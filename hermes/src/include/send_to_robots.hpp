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
        void stm_connect();
        void send_control();

        grSim_Commands commands;
        grSim_Packet packet_grsim;

        int serial_port;
        const char* port = "/dev/ttyUSB";
        uint8_t start[2] = {0xAA, 0xBB};
        uint8_t msg[sizeof(Pacote) + 2];
};

extern robots_sender sender;
//extern grSim_Packet packet_grsim;
//extern grSim_Commands commands;

#endif // SENDO_TO_GRSIM_HPPs