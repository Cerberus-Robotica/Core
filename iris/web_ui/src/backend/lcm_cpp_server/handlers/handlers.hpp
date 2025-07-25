#pragma once

#include <mutex>
#include <vector>
#include <string>
#include <lcm/lcm-cpp.hpp>

#include "../../../../../../data_lcm/data/game_controller.hpp"
#include "../../../../../../data_lcm/data/vision.hpp"
#include "../../../../../../data_lcm/data/tartarus.hpp"
#include "../../../../../../data_lcm/data/ia.hpp"

struct TeamInfo {
    std::string name;
    int score = 0;
    int fouls = 0;
    int goalkeeper_id = 0;
};

struct RobotData {
    int16_t id;
    bool spinner;
    bool kick;
    float vel_tang;
    float vel_normal;
    float vel_ang;
    float kick_speed_x;
    float kick_speed_z;
    bool wheel_speed;
    float wheel_fr;
    float wheel_fl;
    float wheel_bl;
    float wheel_br;
};

struct DetectionRobots {
    int16_t robot_id;
    float position_x;
    float position_y;
    float orientation;
};

struct DetectionBalls {
    float position_x;
    float position_y;
};

struct DetectionGeometry {
    int32_t field_length;
    int32_t field_width;
    int32_t goal_width;
    int32_t goal_depth;
    int32_t boundary_width;
    int32_t center_circle_radius;
    int32_t defense_area_width;
    int32_t defense_area_height;
    int32_t line_thickness;
    int32_t goal_center_to_penalty_mark;
    int32_t goal_height;
    float ball_radius;
    float max_robot_radius;
};

struct LatestData {
    // GC
    bool team_blue = false;
    float designated_position_x = 0;
    float designated_position_y = 0;
    int current_command = 0;
    int game_event = 0;
    TeamInfo yellow;
    TeamInfo blue;

    // IA
    int robots_size = 0;
    std::string processo;
    std::string estrategia;
    std::vector<RobotData> robots;

    // Vision
    int64_t timestamp = 0;
    int16_t robots_yellow_size;
    int16_t robots_blue_size;
    std::vector<DetectionRobots> robots_yellow;
    std::vector<DetectionRobots> robots_blue;
    DetectionBalls balls;
    DetectionGeometry field;

    // Tartarus
    bool ssl_vision = false;
    bool competition_mode = false;
    bool bool_controller = false;
    bool team_blue_status = false;
    int16_t stm_port;
    int16_t controller_port;
};

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

    void handleGC(const lcm::ReceiveBuffer*, const std::string&, const gc_t*);
    void handleVision(const lcm::ReceiveBuffer*, const std::string&, const vision_t*);
    void handleTartarus(const lcm::ReceiveBuffer*, const std::string&, const tartarus_t*);
    void handleIA(const lcm::ReceiveBuffer*, const std::string&, const ia_t*);
};
