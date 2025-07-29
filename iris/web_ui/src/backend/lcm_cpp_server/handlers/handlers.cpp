#include "handlers.hpp"
#include <algorithm>
#include <iostream>

extern std::mutex data_mutex;
extern LatestData latest_data;

LCMControl lcm_control;

void Handler::handleGC(const lcm::ReceiveBuffer *, const std::string &, const gc_t *msg)
{
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_GC = *msg;

    if (lcm_control.team_blue_from_lcm) {
        latest_data.team_blue = msg->team_blue;
    }

    latest_data.designated_position_x = msg->designated_position_x;
    latest_data.designated_position_y = msg->designated_position_y;
    latest_data.current_command = msg->current_command;
    latest_data.game_event = msg->game_event;

    latest_data.blue = msg->blue;
    latest_data.yellow = msg->yellow;
}

void Handler::handleTartarus(const lcm::ReceiveBuffer *, const std::string &, const tartarus_t *msg)
{
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_tartarus = *msg;

    if (lcm_control.goalkeeper_id_from_lcm) {
        if (msg_GC.blue.name == "Cerberus") {
            latest_data.team_blue_status = true;
            latest_data.blue.goalkeeper_id = msg->goalkeeper_id;
        } else if (msg_GC.yellow.name == "Cerberus") {
            latest_data.team_blue_status = false;
            latest_data.yellow.goalkeeper_id = msg->goalkeeper_id;
        } else {
            latest_data.team_blue_status = true;
            latest_data.blue.goalkeeper_id = msg->goalkeeper_id;
        }
    }

    latest_data.ssl_vision = msg->ssl_vision;
    latest_data.competition_mode = msg->competition_mode;
    latest_data.bool_controller = msg->bool_controller;
    latest_data.stm_port = msg->stm_port;
    latest_data.controller_port = msg->controller_port;
}


void Handler::handleVision(const lcm::ReceiveBuffer *, const std::string &, const vision_t *msg)
{
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_vision = *msg;
    latest_data.timestamp = msg->timestamp;

    // Copiar robôs amarelos
    latest_data.robots_yellow.clear();
    for (int i = 0; i < msg->robots_yellow_size; ++i)
    {
        latest_data.robots_yellow.push_back(msg->robots_yellow[i]);
    }
    latest_data.robots_yellow_size = msg->robots_yellow_size;

    // Copiar robôs azuis
    latest_data.robots_blue.clear();
    for (int i = 0; i < msg->robots_blue_size; ++i)
    {
        latest_data.robots_blue.push_back(msg->robots_blue[i]);
    }
    latest_data.robots_blue_size = msg->robots_blue_size;

    latest_data.balls = msg->balls;
    latest_data.field = msg->field;
}

void Handler::handleIA(const lcm::ReceiveBuffer *, const std::string &, const ia_t *msg)
{
    std::lock_guard<std::mutex> lock(data_mutex);
    msg_ia = *msg;

    latest_data.robots_size = msg->robots_size;
    latest_data.processo = msg->processo;
    latest_data.estrategia = msg->estrategia;
    latest_data.timestamp = msg->timestamp;

    int safe_size = std::max(0, std::min(static_cast<int>(msg->robots_size), 16));

    latest_data.robots.clear();
    for (int i = 0; i < safe_size; ++i) {
        latest_data.robots.push_back(msg->robots[i]);
    }
}

