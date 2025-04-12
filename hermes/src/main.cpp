#include "include/socket_connect.hpp"
#include "include/handler.hpp"
#include "include/esp.hpp"
#include "include/send_to_grsim.hpp"
#include <thread>

grsim_sender grsim_send;

int main(int argc, char** argv) {

    setupSocket_grsim();

    lcm::LCM lcm;
    if (!lcm.good())
        return 1;

    lcm.subscribe("IA", &handler::handleIA, &han);
    lcm.subscribe("tartarus", &handler::handletartarus, &han);

    std::thread t1([&]() { 
        grsim_send.send_to_grsim(); 
    });

    std::thread lcm_thread([&lcm]() {
        while (true) {
            lcm.handle();
        }
    });

    t1.join();
    lcm_thread.join();

    close(sock_grsim);
    return 0;
}
