#ifndef CONTROL_GRSIM_HPP
#define CONTROL_GRSIM_HPP

#include "handler.hpp"
#include "../../build/ssl_simulation_control.pb.h"
#include "socket_connect.hpp"
#include "stm.hpp"
#include "manual_control.hpp"
#include <chrono>

class grsim_control {
    public:
        void grsim_controller();

        SimulatorCommand command;
        SimulatorControl control;

        bool updated = false;
};

extern grsim_control control_grsim;

#endif // CONTROL_GRSIM_HPP