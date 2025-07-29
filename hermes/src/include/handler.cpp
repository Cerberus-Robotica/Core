#include "handler.hpp"
#include "send_to_robots.hpp"

handler han;

void handler::handleIA(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::ia * data_ia) {

    this->data_ia_copy = *data_ia;

}

void handler::handletartarus(const lcm::ReceiveBuffer* rbuf,
    const std::string& chan, 
    const data::tartarus * data_tartarus) {

    this->data_tartarus_copy = *data_tartarus;

    updated_tartarus = !updated_tartarus;

    std::cout << "tartarus has been updated: " << std::boolalpha << updated_tartarus << std::endl;
}