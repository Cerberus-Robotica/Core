// handlers.cpp
#include "handlers.hpp"
#include <algorithm>
#include <iostream>

extern std::mutex data_mutex;
extern LatestData latest_data;

void Handler::handleGC(const lcm::ReceiveBuffer*, const std::string&, const gc_t* msg) {
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_GC = *msg;
    latest_data.team_blue = msg_GC.team_blue;
    latest_data.designated_position_x = msg_GC.designated_position_x;
    latest_data.designated_position_y = msg_GC.designated_position_y;
    latest_data.current_command = msg_GC.current_command;
    latest_data.game_event = msg_GC.game_event;

    latest_data.blue.name = msg_GC.blue.name;
    latest_data.blue.score = msg_GC.blue.score;
    latest_data.blue.fouls = msg_GC.blue.fouls;
    latest_data.blue.goalkeeper_id = msg_GC.blue.goalkeeper_id;

    latest_data.yellow.name = msg_GC.yellow.name;
    latest_data.yellow.score = msg_GC.yellow.score;
    latest_data.yellow.fouls = msg_GC.yellow.fouls;
    latest_data.yellow.goalkeeper_id = msg_GC.yellow.goalkeeper_id;
}

void Handler::handleVision(const lcm::ReceiveBuffer*, const std::string&, const vision_t* msg) {
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_vision = *msg;
    latest_data.timestamp = msg_vision.timestamp;

    latest_data.robots_yellow.clear();
    latest_data.robots_yellow_size = msg_vision.robots_yellow_size;
    for (int i = 0; i < msg_vision.robots_yellow_size; ++i) {
        DetectionRobots dr;
        dr.robot_id = msg_vision.robots_yellow[i].robot_id;
        dr.position_x = msg_vision.robots_yellow[i].position_x;
        dr.position_y = msg_vision.robots_yellow[i].position_y;
        dr.orientation = msg_vision.robots_yellow[i].orientation;
        latest_data.robots_yellow.push_back(dr);
    }

    latest_data.robots_blue.clear();
    latest_data.robots_blue_size = msg_vision.robots_blue_size;
    for (int i = 0; i < msg_vision.robots_blue_size; ++i) {
        DetectionRobots db;
        db.robot_id = msg_vision.robots_blue[i].robot_id;
        db.position_x = msg_vision.robots_blue[i].position_x;
        db.position_y = msg_vision.robots_blue[i].position_y;
        db.orientation = msg_vision.robots_blue[i].orientation;
        latest_data.robots_blue.push_back(db);
    }

    latest_data.balls.position_x = msg_vision.balls.position_x;
    latest_data.balls.position_y = msg_vision.balls.position_y;

    latest_data.field.field_length = msg_vision.field.field_length;
    latest_data.field.field_width = msg_vision.field.field_width;
    latest_data.field.goal_width = msg_vision.field.goal_width;
    latest_data.field.goal_depth = msg_vision.field.goal_depth;
    latest_data.field.boundary_width = msg_vision.field.boundary_width;
    latest_data.field.center_circle_radius = msg_vision.field.center_circle_radius;
    latest_data.field.defense_area_width = msg_vision.field.defense_area_width;
    latest_data.field.defense_area_height = msg_vision.field.defense_area_height;
    latest_data.field.line_thickness = msg_vision.field.line_thickness;
    latest_data.field.goal_center_to_penalty_mark = msg_vision.field.goal_center_to_penalty_mark;
    latest_data.field.goal_height = msg_vision.field.goal_height;
    latest_data.field.ball_radius = msg_vision.field.ball_radius;
    latest_data.field.max_robot_radius = msg_vision.field.max_robot_radius;
}

void Handler::handleTartarus(const lcm::ReceiveBuffer*, const std::string&, const tartarus_t* msg) {
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_tartarus = *msg;

    if (msg_GC.blue.name == "Cerberus") {
        latest_data.team_blue_status = true;
        latest_data.blue.goalkeeper_id = msg_tartarus.goalkeeper_id;
    } else if (msg_GC.yellow.name == "Cerberus") {
        latest_data.team_blue_status = false;
        latest_data.yellow.goalkeeper_id = msg_tartarus.goalkeeper_id;
    } else {
        latest_data.team_blue_status = true;
        latest_data.blue.goalkeeper_id = msg_tartarus.goalkeeper_id;
    }

    latest_data.ssl_vision = msg_tartarus.ssl_vision;
    latest_data.competition_mode = msg_tartarus.competition_mode;
    latest_data.bool_controller = msg_tartarus.bool_controller;
    latest_data.stm_port = msg_tartarus.stm_port;
    latest_data.controller_port = msg_tartarus.controller_port;
}

void Handler::handleIA(const lcm::ReceiveBuffer*, const std::string&, const ia_t* msg) {
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_ia = *msg;

    latest_data.robots_size = msg->robots_size;
    latest_data.processo = msg->processo;
    latest_data.estrategia = msg->estrategia;
    latest_data.timestamp = msg->timestamp;

    int safe_size = std::max(0, std::min(static_cast<int>(msg->robots_size), 16));

    latest_data.robots.clear();
    for (int i = 0; i < safe_size; ++i) {
        const auto& robot = msg->robots[i];
        RobotData r;
        r.id = robot.id;
        r.spinner = robot.spinner;
        r.kick = robot.kick;
        r.vel_tang = robot.vel_tang;
        r.vel_normal = robot.vel_normal;
        r.vel_ang = robot.vel_ang;
        r.kick_speed_x = robot.kick_speed_x;
        r.kick_speed_z = robot.kick_speed_z;
        r.wheel_speed = robot.wheel_speed;
        r.wheel_fr = robot.wheel_fr;
        r.wheel_fl = robot.wheel_fl;
        r.wheel_bl = robot.wheel_bl;
        r.wheel_br = robot.wheel_br;

        latest_data.robots.push_back(r);
    }
}
