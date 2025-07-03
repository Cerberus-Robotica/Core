#include "handler.hpp"
#include "stm.hpp"

handler han;

void handler::handleIA(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::ia * data_ia) {

    this->data_ia_copy = *data_ia;
    pct.id = data_ia->robots[0].id;
    pct.Vx = data_ia->robots[0].vx;
    pct.Vy = data_ia->robots[0].vy;
    pct.Vang = data_ia->robots[0].vang;
    pct.kicker = data_ia->robots[0].kick_speed_x;
}

void handler::handletartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::tartarus * data_tartarus) {

    this->data_tartarus_copy = *data_tartarus;
}