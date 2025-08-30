#ifndef VISION_MASTER_HPP
#define VISION_MASTER_HPP

#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <unordered_set>
#include <thread>
#include "socket_connect.hpp"
#include "../../data_lcm/vision.hpp"
#include "../../build/ssl_vision_wrapper.pb.h"
#include "../../data_lcm/autoref.hpp"
#include "../../build/ssl_vision_wrapper_tracked.pb.h"

class vision_master
{
public:
    void vision_main();
    void recebe_dados_tracked();
    void recebe_dados_vision();
    void tartarus_updated();

    lcm::LCM lcm;
    bool ssl_vision_atual;
    bool autoreferee_atual;
    int cameras;
    std::unordered_set<int> yellow_ids;
    std::unordered_set<int> blue_ids; // Conjunto para armazenar IDs únicos, de robôs
    bool updated = false;
};

extern vision_master vision_master_instance;
extern data::vision my_vision_data;
extern data::autoref my_autoref_data;

#endif