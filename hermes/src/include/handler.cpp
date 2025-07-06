#include "handler.hpp"
#include "send_to_robots.hpp"

handler han;

void handler::handleIA(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::ia * data_ia) {

    this->data_ia_copy = *data_ia;
    
    if(han.data_tartarus_copy.ssl_vision == 1) {
        pct.id = data_ia->robots[0].id;
        pct.Vx = data_ia->robots[0].vel_tang; //vx é o vel_tang
        pct.Vy = data_ia->robots[0].vel_normal; //vy é o vel_normal
        pct.Vang = data_ia->robots[0].vel_ang;
        pct.kicker = data_ia->robots[0].kick_speed_x;
    }
}

void handler::handletartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::tartarus * data_tartarus) {

    this->data_tartarus_copy = *data_tartarus;

}