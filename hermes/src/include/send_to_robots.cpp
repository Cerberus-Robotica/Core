#include "send_to_robots.hpp"
#include <thread>

bool time_atual;
Pacote pct;

void robots_sender::send_to_grsim() { // function to send data to grSim
    commands.Clear();
    std::cout << "team robot size " << han.data_ia_copy.robots_size << std::endl;
    std::cout << "timestamp " << han.data_ia_copy.timestamp << std::endl;

    if (han.data_ia_copy.robots_size == 0) {
        std::cerr << "Warning: robots_size is 0, skipping send_to_grsim()" << std::endl;
        return;
    }

    if(han.data_gc_copy.team_blue == time_atual)
    {
        for (int i = 0; i < han.data_ia_copy.robots_size; i++) {
            const data::robot * robot_ia = &han.data_ia_copy.robots[i];
            auto robotpacket = commands.add_robot_commands();
            robotpacket->set_id(robot_ia->id);
            robotpacket->set_wheelsspeed(robot_ia->wheel_speed);
            robotpacket->set_kickspeedx(robot_ia->kick_speed_x);
            robotpacket->set_kickspeedz(robot_ia->kick_speed_z);
            robotpacket->set_veltangent(robot_ia->vel_tang);
            robotpacket->set_velnormal(robot_ia->vel_normal);
            robotpacket->set_velangular(robot_ia->vel_ang);
            robotpacket->set_spinner(robot_ia->spinner);
            robotpacket->set_wheel1(robot_ia->wheel_fl);//certo
            robotpacket->set_wheel2(robot_ia->wheel_bl);
            robotpacket->set_wheel3(robot_ia->wheel_br);
            robotpacket->set_wheel4(robot_ia->wheel_fr);//certo

        }
        commands.set_timestamp(han.data_ia_copy.timestamp);
        commands.set_isteamyellow(!han.data_gc_copy.team_blue);
    }
    else{
        //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (int i = 0; i < han.data_ia_copy.robots_size; i++) {
            const data::robot * robot_ia = &han.data_ia_copy.robots[i];
            auto robotpacket = commands.add_robot_commands();
            robotpacket->set_id(robot_ia->id);
            robotpacket->set_wheelsspeed(true); //bool
            robotpacket->set_kickspeedx(robot_ia->kick_speed_x);
            robotpacket->set_kickspeedz(robot_ia->kick_speed_z);
            robotpacket->set_veltangent(robot_ia->vel_tang);
            robotpacket->set_velnormal(robot_ia->vel_normal);
            robotpacket->set_velangular(robot_ia->vel_ang);
            robotpacket->set_spinner(robot_ia->spinner); //bool
            robotpacket->set_wheel1(0);//certo
            robotpacket->set_wheel2(0);
            robotpacket->set_wheel3(0);
            robotpacket->set_wheel4(0);//certo

        }
        commands.set_timestamp(han.data_ia_copy.timestamp);
        commands.set_isteamyellow(han.data_gc_copy.team_blue);
        time_atual = han.data_gc_copy.team_blue;
    }

    packet_grsim.mutable_commands()->CopyFrom(commands);
    std::cout << "commands size " << packet_grsim.commands().robot_commands_size() << std::endl;

    std::cout << "byte size " << packet_grsim.ByteSize() << std::endl << std::endl;
    packet_grsim.SerializeAsString();
    std::string serialized_packet = packet_grsim.SerializeAsString();
    sendto(sock_grsim, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr_grsim, sizeof(addr_grsim));
    //std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "Sending to grSim..." << std::endl << std::endl;


}

void robots_sender::send_control() { // global function to send control commands
    setupSocket_grsim();
    while(true) {
        if(han.updated_tartarus != sender.updated) {
            sender.updated = !sender.updated;
        }
        //control_obj.connect_controller(); // Conecta o controle
        while(han.data_tartarus_copy.bool_controller == 1 && control_obj.joy == nullptr) { //only works with UI
            control_obj.connect_controller(); // try to connect the controller
            std::cout << "trying to connect the controller..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        if (han.data_tartarus_copy.ssl_vision == 0) {
            close(stm_obj.serial_port);

            while(han.data_tartarus_copy.ssl_vision == 0 && han.updated_tartarus == sender.updated) {
                if(han.updated_tartarus != sender.updated) {
                    sender.updated = !sender.updated;
                }
                if(han.data_tartarus_copy.competition_mode == 0) {
                    control_obj.control(); // Mantém atualizando

                    for (int i = 0; i < han.data_ia_copy.robots_size; i++) {
                        data::robot* r = &han.data_ia_copy.robots[i];
                        if (r->id == control_obj.robot_id) {
                            r->vel_tang = pct.Vy;
                            r->vel_normal = pct.Vx;
                            r->vel_ang = pct.Vang;
                            std::cout << "Controlled robot - " << "Robot I: " << (int)r->id << " Vy: " << r->vel_tang << " Vx: " << r->vel_normal << " Vang: " << r->vel_ang << std::endl;
                        }
                        else {
                            r->vel_tang = 0;
                            r->vel_normal = 0;
                            r->vel_ang = 0;
                        }
                    }
                }
                send_to_grsim();
            }
        } else {
            stm_obj.stm_connect();
            while(han.data_tartarus_copy.ssl_vision == 1 && han.updated_tartarus == sender.updated) {
                if(han.updated_tartarus != sender.updated) {
                    sender.updated = !sender.updated;
                }

                if(han.data_tartarus_copy.competition_mode == 0) {
                    control_obj.control(); // Mantém atualizando
                    pct.id = control_obj.robot_id; // Use the current robot ID from the controller
                    std::cout << "Controlled robot - Robot ID: " << (int)pct.id << " Vx: " << pct.Vx << " Vy: " << pct.Vy << " Vang: " << pct.Vang << std::endl;
                    memcpy(&stm_obj.msg[2], &pct, sizeof(Pacote));
                    write(stm_obj.serial_port, stm_obj.msg, sizeof(stm_obj.msg));
                    usleep(5000);
                }
                else{
                    for (int i = 0; i < 16; i++) {
                        if(han.updated_tartarus != sender.updated) {
                            sender.updated = !sender.updated;
                        }

                        else {
                            pct.id = han.data_ia_copy.robots[i].id;
                            pct.Vx = han.data_ia_copy.robots[i].vel_tang; //vx é o vel_tang
                            pct.Vy = han.data_ia_copy.robots[i].vel_normal; //vy é o vel_normal
                            pct.Vang = -han.data_ia_copy.robots[i].vel_ang;
                            pct.kicker = han.data_ia_copy.robots[i].kick_speed_x;
                        }
                        std::cout << "Controlled robot - Robot ID: " << (int)pct.id << " Vx: " << pct.Vx << " Vy: " << pct.Vy << " Vang: " << pct.Vang << std::endl;
                        memcpy(&stm_obj.msg[2], &pct, sizeof(Pacote));
                        write(stm_obj.serial_port, stm_obj.msg, sizeof(stm_obj.msg));
                        usleep(5000);
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    close(stm_obj.serial_port);
    close(sock_grsim);
}
