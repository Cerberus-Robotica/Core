#include "handler.hpp"

handler han;

void handler::handleIA(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::ia * data_ia) {

    std::cout << "robot size " << data_ia->robots_size << std::endl;
    std::cout << "timestamp " << data_ia->timestamp << std::endl;
    for (int i = 0; i < data_ia->robots_size; i++) {
        const data::robot * robot_ia = &data_ia->robots[i];
        auto robotpacket = this->commands.add_robot_commands();
        robotpacket->set_id(robot_ia->id);
        robotpacket->set_wheelsspeed(robot_ia->wheel_speed);
        robotpacket->set_kickspeedx(robot_ia->kick_speed_x);
        robotpacket->set_kickspeedz(robot_ia->kick_speed_z);
        robotpacket->set_veltangent(robot_ia->vel_tang);
        robotpacket->set_velnormal(robot_ia->vel_normal);
        robotpacket->set_velangular(robot_ia->vel_ang);
        robotpacket->set_spinner(robot_ia->spinner);
        robotpacket->set_wheel1(robot_ia->wheel_fl);//certo
        robotpacket->set_wheel2(robot_ia->wheel_bl);
        robotpacket->set_wheel3(robot_ia->wheel_br);
        robotpacket->set_wheel4(robot_ia->wheel_fr);//certo

    }
    this->commands.set_timestamp(data_ia->timestamp); // Use set_timestamp() method
    this->commands.set_isteamyellow(false); // Use set_isteamyellow() method

}

void handler::handletartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::tartarus * data_tartarus) {
    std::cout << "Received tartarus message" << std::endl;
    // Handle tartarus message
    //std::cout << "estrategia " << data_tartarus->estrategia << std::endl;
    //std::cout << "processo " << data_tartarus->processo << std::endl;
    //std::cout << "ssl_vision " << data_tartarus->ssl_vision << std::endl;
}