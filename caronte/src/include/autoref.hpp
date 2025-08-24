#ifndef AUTOREF_HPP
#define AUTOREF_HPP

#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/autoref.hpp"
#include "../../build/ssl_vision_wrapper_tracked.pb.h"

extern data::autoref my_autoref_data;
void recebe_dados_tracked();

#endif