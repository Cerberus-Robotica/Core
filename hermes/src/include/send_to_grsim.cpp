#include "send_to_grsim.hpp"
#include <thread>

bool time_atual;

void grsim_sender::send_to_grsim()
{
    while(true)
    {
        commands.Clear();
        std::cout << "team robot size " << han.data_ia_copy.robots_size << std::endl;
        std::cout << "timestamp " << han.data_ia_copy.timestamp << std::endl;
        if(han.data_tartarus_copy.team_blue == time_atual)
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
            commands.set_isteamyellow(!han.data_tartarus_copy.team_blue); 
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
            commands.set_isteamyellow(han.data_tartarus_copy.team_blue);
            time_atual = han.data_tartarus_copy.team_blue;
        }




        packet_grsim.mutable_commands()->CopyFrom(commands);
        std::cout << "commands size " << packet_grsim.commands().robot_commands_size() << std::endl;

        std::cout << "byte size " << packet_grsim.ByteSize() << std::endl << std::endl;
        packet_grsim.SerializeAsString();
        std::string serialized_packet = packet_grsim.SerializeAsString();
        sendto(sock_grsim, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr_grsim, sizeof(addr_grsim));
        std::cout << "Sending to grSim..." << std::endl;
        

    }

}