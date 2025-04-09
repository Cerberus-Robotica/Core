#ifndef GC_HPP
#define GC_HPP

#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/data/game_controller.hpp"
#include "../../build/ssl_gc_referee_message.pb.h"

extern data::game_controller my_gc_data;
void recebe_dados_GC();

#endif