#include "include/socket_connect.hpp"
#include "include/handler.hpp"
#include "include/esp.hpp"
#include "include/send_to_robots.hpp"
#include "include/manual_control.hpp"
#include "stm.hpp"
#include <thread>

robots_sender sender;
stm stm_obj;

int main(int argc, char** argv) {

    lcm::LCM lcm;
    if (!lcm.good())
        return 1;

    lcm.subscribe("IA", &handler::handleIA, &han);
    lcm.subscribe("tartarus", &handler::handletartarus, &han);

    std::thread t1([&]() { 
        sender.send_control(); 
    });

    std::thread lcm_thread([&lcm]() {
        while (true) {
            lcm.handle();
        }
    });

    t1.join();
    lcm_thread.join();

    close(sock_grsim);
    close(stm_obj.serial_port);
    return 0;
}
