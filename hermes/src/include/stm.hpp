#ifndef STM_HPP
#define STM_HPP

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>
#include "handler.hpp"

#pragma pack(push, 1)
typedef struct {
    uint8_t id;
    float Vx, Vy, Vang;
    uint8_t kicker;
} Pacote;
#pragma pack(pop)

class stm {
    public:
        void stm_connect(); // function to connect to the STM32

        int serial_port;
        const char* port = "/dev/ttyUSB";
        uint8_t start[2] = {0xAA, 0xBB};
        uint8_t msg[sizeof(Pacote) + 2];
};

extern Pacote pct;
extern stm stm_obj;

#endif