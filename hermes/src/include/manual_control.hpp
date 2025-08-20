#ifndef MANUAL_CONTROL_HPP
#define MANUAL_CONTROL_HPP

#include <SDL2/SDL.h>
#include "handler.hpp"
#include "stm.hpp"

class controller{
public:
    // the value returned by SDL (SDL_JOYAXISMOTION) is in a range of integers between -32768 and 32767
    float normalizeAxis(int16_t value) {
        const float deadzone = 8000; //define 8000 as threshold for deadzone
        if (std::abs(value) < deadzone) return 0.0f; // abs(value) returns value in module, if within deadzone, return 0, no movement
        return value / 30720.0f; // Normalize to range [-(32768.0/10000.0), 32767.0/10000.0] <-- velocity range
    }

    void control();
    void connect_controller();

    SDL_Event e;
    SDL_Joystick* joy;

    int robot_id = 0; // Variable to store the current robot ID
    bool try_connect; // Variable to control connection attempts
};

extern controller control_obj;

#endif