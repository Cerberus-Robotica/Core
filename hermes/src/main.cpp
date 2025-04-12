#include "include/socket_connect.hpp"
#include "include/handler.hpp"
#include "include/esp.hpp"
#include "include/send_to_grsim.hpp"
#include <thread>


int main(int argc, char** argv) {

    setupSocket_grsim();

    lcm::LCM lcm;
    if (!lcm.good())
        return 1;

    lcm.subscribe("IA", &handler::handleIA, &han);
    lcm.subscribe("tartarus", &handler::handletartarus, &han);

    std::thread lcm_thread([&lcm]() {
        while (true) {
            lcm.handle();
        }
    });

    lcm_thread.join();

    close(sock_grsim);
    return 0;
}
