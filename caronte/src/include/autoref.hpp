void recebe_dados_tracked();

#ifndef AUTOREF_HPP
#define AUTOREF_HPP

#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/data/autoref.hpp"
#include "../../build/ssl_vision_wrapper_tracked.pb.h"

extern data::autoref my_autoref_data;
void recebe_dados_vision();

#endif