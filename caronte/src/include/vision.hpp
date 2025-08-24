#ifndef VISION_HPP
#define VISION_HPP

#include <lcm/lcm-cpp.hpp>
#include "../../data_lcm/vision.hpp"
#include "../../build/ssl_vision_wrapper.pb.h"

extern data::vision my_vision_data;
void recebe_dados_vision();

#endif