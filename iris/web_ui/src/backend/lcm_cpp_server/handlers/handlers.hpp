#pragma once

#include <mutex>
#include <string>
#include <lcm/lcm-cpp.hpp>

#include "../../../../../../data_lcm/game_controller.hpp"
#include "../../../../../../data_lcm/vision.hpp"
#include "../../../../../../data_lcm/tartarus.hpp"
#include "../../../../../../data_lcm/ia.hpp"

// Agora LatestData utiliza diretamente as structs LCM, sem redefini-las

struct LatestData {
    // Game Controller
    bool team_blue = false;
    float designated_position_x = 0.f;
    float designated_position_y = 0.f;
    int current_command = 0;
    int game_event = 0;
    data::team_info yellow;
    data::team_info blue;

    // IA
    int robots_size = 0;
    std::string processo;
    std::string estrategia;
    std::vector<data::robot> robots;  // supondo que 'robot' esteja definido no ia.hpp

    // Vision
    int64_t timestamp = 0;
    int16_t robots_yellow_size = 0;
    int16_t robots_blue_size = 0;
    std::vector<data::detection_robots> robots_yellow;
    std::vector<data::detection_robots> robots_blue;
    data::detection_balls balls;
    data::detection_geometry field;

    // Tartarus
    bool autoreferee = false;
    bool ssl_vision = false;
    bool competition_mode = false;
    bool bool_controller = false;
    bool team_blue_status = false;
    int16_t stm_port = 0;
    int16_t controller_port = 0;
};

struct LCMControl {
    bool goalkeeper_id_from_lcm = true;
    bool team_blue_from_lcm = true;
};

extern LCMControl lcm_control;

extern LatestData latest_data;
extern std::mutex data_mutex;

class Handler {
public:
    using gc_t = data::game_controller;
    using vision_t = data::vision;
    using tartarus_t = data::tartarus;
    using ia_t = data::ia;

    gc_t msg_GC;
    vision_t msg_vision;
    tartarus_t msg_tartarus;
    ia_t msg_ia;

    void handleGC(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const gc_t* msg);
    void handleVision(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const vision_t* msg);
    void handleTartarus(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const tartarus_t* msg);
    void handleIA(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const ia_t* msg);
};
