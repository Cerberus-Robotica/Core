#include <thread>
#include "include/handlers.hpp"

handlers han;

int main() {
    lcm::LCM lcm;
    if (!lcm.good()) {
        std::cerr << "LCM is not good" << std::endl;
        return 1;
    }

    han.lc = &lcm;

    lcm.subscribe("GC", &handlers::handle_GC, &han);
    lcm.subscribe("vision", &handlers::handle_ia_vision, &han);
    lcm.subscribe("tartarus", &handlers::handle_tartarus, &han);

    han.lc->publish("tartarus", &han.new_tartarus); //inicializa o tartarus

    std::cout << "Waiting for messages on channels" << std::endl;

    //thread para receber e publicar nos topicos de mensagem
    std::thread lcm_thread([&lcm]() {
        while (true) {
            //system("clear");
            //std::cout << "\n----------------------------------------\n";
            //std::cout << "            C E R B E R U S \n";
            //std::cout << "----------------------------------------" << std::endl;
            //std::cout << "   Timestamp sslvision   " << han.new_ia.timestamp << " \n" << std::endl;
            //std::cout << "   time_azul = " << (int)han.new_GC.team_blue << "\n" << std::endl; // 0 para false, 1 para true
            //std::cout << "   processo: " << han.new_tartarus.processo << std::endl;
            //std::cout << "   estrategia: " << han.new_tartarus.estrategia << std::endl;
            //std::cout << "   ssl_vision: " << (int)han.new_tartarus.ssl_vision << std::endl; // 0 para false, 1 para true
            //std::cout << "----------------------------------------" << std::endl;
            //std::cout << "   robots_yellow_size = " << han.new_ia.robots_size << std::endl;
            //std::cout << "   robots_blue_size = " << han.new_ia.robots_size << "\n" << std::endl;
            //std::cout << "   Field Length: " << han.new_vision.field.field_length << std::endl;
            lcm.handle();
        }
    });

    lcm_thread.join();

    std::cout << "Shutting down..." << std::endl;
    return 0;
}