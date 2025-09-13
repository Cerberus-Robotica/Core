#include "include/control_grsim.hpp"
#include <thread>

using namespace std::chrono;

void grsim_control::grsim_controller(){
    std::cout << "AAAAAAAAAAA" << std::endl;
    setupSocket_control_grsim();
    std::cout << "BBBBBBBBB" << std::endl;
    std::string serialized_packet;
    steady_clock::time_point start = steady_clock::now();
    const auto intervalo = 2000ms; 
    
    while(!han.data_tartarus_copy.ssl_vision) {
        std::cout << "game event: "<< han.data_gc_copy.game_event << std::endl;
        control.set_simulation_speed(1.0);
        if(han.updated_tartarus != control_grsim.updated) {
            control_grsim.updated = han.updated_tartarus;
            start = steady_clock::now();
        }
        if(steady_clock::now() - start < intervalo) {
            auto teleport = control.mutable_teleport_ball();
            if(han.data_gc_copy.game_event == 8){
                teleport->set_x(0.0);
                teleport->set_y(0.0);
                teleport->set_z(0.0);
                teleport->set_vx(0.0);
                teleport->set_vy(0.0);
            }
            std::cout << "start: " << duration_cast<seconds>(steady_clock::now() - start).count() << std::endl;
        }
        command.mutable_control()->CopyFrom(control);
        std::cout << command.control().teleport_ball().x() << std::endl;
        std::cout << command.control().teleport_ball().y() << std::endl;
        std::cout << command.control().teleport_ball().vx() << std::endl;
        std::cout << command.control().teleport_ball().vy() << std::endl;
        std::cout << "command size: " << command.ByteSize() << std::endl;

        //std::cout << "byte size " << packet_grsim.ByteSize() << std::endl << std::endl;
        serialized_packet = command.SerializeAsString();
        std::cout << "Sending packet to grsim..." << std::endl;
        sendto(sock_control_grsim, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr_control_grsim, sizeof(addr_control_grsim));
        control.Clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }
}