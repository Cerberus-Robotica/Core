#include "include/control_grsim.hpp"
#include <thread>

using namespace std::chrono;

void grsim_control::grsim_controller(){
    std::string serialized_packet;
    steady_clock::time_point start = steady_clock::now();
    const auto intervalo = 2000ms;
    
    while(!han.data_tartarus_copy.ssl_vision) {
        if(han.updated_tartarus != control_grsim.updated) {
            setupSocket_control_grsim();
            control_grsim.updated = han.updated_tartarus;
            start = steady_clock::now();
        
            if(steady_clock::now() - start < intervalo) {
                if(han.data_gc_copy.has_designated_position){
                    auto teleport = control.mutable_teleport_ball();
                    teleport->set_x(han.data_gc_copy.designated_position_x);
                    teleport->set_y(han.data_gc_copy.designated_position_y);
                    teleport->set_z(0.0);
                    teleport->set_vx(0.0);
                    teleport->set_vy(0.0);
                }
                else if(han.data_gc_copy.game_event == 8){
                    auto teleport = control.mutable_teleport_ball();
                    teleport->set_x(0.0);
                    teleport->set_y(0.0);
                    teleport->set_z(0.0);
                    teleport->set_vx(0.0);
                    teleport->set_vy(0.0);
                }

                command.mutable_control()->CopyFrom(control);
            }
            std::cout << "designated position x: "<< han.data_gc_copy.designated_position_x << std::endl;
            std::cout << "designated position y: "<< han.data_gc_copy.designated_position_y << std::endl;
            std::cout << "game event: "<< han.data_gc_copy.game_event << std::endl;

            serialized_packet = command.SerializeAsString();

            sendto(sock_control_grsim, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr_control_grsim, sizeof(addr_control_grsim));
            control.Clear();
            command.Clear();
            close(sock_control_grsim);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}