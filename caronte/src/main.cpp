#include "include/socket_connect.hpp"
#include "include/vision_master.hpp"
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

    han.lc = &lcm;

    //configura o lcm para receber do tópico "tartarus" pelo endereço especificado
    lcm.subscribe("tartarus", &Handler::handle_tartarus, &han);

    setupGCSocket();
    setupVisionSocket();
    setupTrackedSocket();

    // start GC receiver thread
    std::thread t1(recebe_dados_GC);

    std::thread t2(&vision_master::vision_main, &vision_master_instance);

    std::thread lcm_thread([&lcm]() { //cria um thread da biblioteca do C para o lcm esperar pelas mensagens
        while (true) {
            lcm.handle();
        }
    });

    t1.join();
    t2.join();
    lcm_thread.join();
    
    close(sock_vision);
    close(sock_GC);
    close(sock_TRACKED);

    return 0;
}
