#include "include/socket_connect.hpp"
#include "include/vision.hpp"
#include "include/GC.hpp"
#include "include/tartarus.hpp"
#include <thread>

int main(int argc, char ** argv)
{

    lcm::LCM lcm;
    if (!lcm.good()) {
        std::cerr << "Erro ao iniciar o LCM." << std::endl;
        return 1;
    }

    Handler handler;
    handler.lc = &lcm;

    lcm.subscribe("tartarus", &Handler::handle_tartarus, &handler);

    setupGCSocket();
    setupVisionSocket();

    std::thread t1(recebe_dados_GC);
    std::thread t2(recebe_dados_vision);
    
    std::thread lcm_thread([&lcm]() {
        while (true) {
            lcm.handle();
        }
    });

    t1.join();
    t2.join();
    lcm_thread.join();
    
    close(sock_vision);
    close(sock_GC);

    return 0;
}