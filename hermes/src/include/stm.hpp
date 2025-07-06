#ifndef STM_HPP
#define STM_HPP

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstring>

#pragma pack(push, 1)
typedef struct {
    uint8_t id;
    float Vx, Vy, Vang;
    uint8_t kicker;
} Pacote;
#pragma pack(pop)

extern Pacote pct;

#endif