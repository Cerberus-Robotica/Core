#include "include/socket_connect.hpp"
#include "include/handler.hpp"
#include "include/esp.hpp"
#include "include/send_to_robots.hpp"
#include "include/manual_control.hpp"
#include "include/control_grsim.hpp"
#include "stm.hpp"
#include <thread>

grsim_control control_grsim;
robots_sender sender;
stm stm_obj;

int main(int argc, char** argv) {

    lcm::LCM lcm;
    if (!lcm.good())
        return 1;

    lcm.subscribe("IA", &handler::handleIA, &han);
    lcm.subscribe("tartarus", &handler::handletartarus, &han);
    lcm.subscribe("GC", &handler::handleGC, &han);

    std::thread t1([&]() { 
        sender.send_control(); 
    });

    std::thread t2([&]() { 
        control_grsim.grsim_controller(); 
    });

    std::thread lcm_thread([&lcm]() {
        while (true) {
            lcm.handle();
        }
    });

    t1.join();
    t2.join();
    lcm_thread.join();

    close(sock_grsim);
    close(sock_control_grsim);
    close(stm_obj.serial_port);
    return 0;
}
