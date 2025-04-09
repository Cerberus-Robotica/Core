#include <lcm/lcm-cpp.hpp>
#include <iostream>
#include <stdio.h>
#include "../../data_lcm/data/ia.hpp"
#include "../../data_lcm/data/tartarus.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include "../build/grSim_Packet.pb.h"

#define PORT_GRSIM 20011
#define IP_GRSIM "127.0.0.1"

int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
grSim_Packet packet_grsim;
struct sockaddr_in addr;

class Handler {
public:
    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const data::ia * data_ia) {

        int i;
        grSim_Commands commands;
        std::cout << "robot size " << data_ia->robots_size << std::endl;
        std::cout << "timestamp " << data_ia->timestamp << std::endl;
        for (i = 0; i < data_ia->robots_size; i++) {
            const data::robot * robot_ia = &data_ia->robots[i];
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
        commands.set_timestamp(data_ia->timestamp); // Use set_timestamp() method
        commands.set_isteamyellow(false); // Use set_isteamyellow() method

        packet_grsim.mutable_commands()->CopyFrom(commands);
        std::cout << "commands size " << packet_grsim.commands().robot_commands_size() << std::endl;
        std::cout << "byte size " << packet_grsim.ByteSize() << std::endl << std::endl;
        packet_grsim.SerializeAsString();
        std::string serialized_packet = packet_grsim.SerializeAsString();
        sendto(sock, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr, sizeof(addr));
        std::cout << "Sending to grSim..." << std::endl;
        
    }

    void handletartarus(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const data::tartarus * data_tartarus) {
        std::cout << "Received tartarus message" << std::endl;
        // Handle tartarus message
        std::cout << "estrategia " << data_tartarus->estrategia << std::endl;
        std::cout << "processo " << data_tartarus->processo << std::endl;
        std::cout << "ssl_vision " << data_tartarus->ssl_vision << std::endl;
    }
};

int main(int argc, char** argv) {

    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_GRSIM);
    inet_pton(AF_INET, IP_GRSIM, &addr.sin_addr);
    
    unsigned char ttl = 2;

    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));



    lcm::LCM lcm;
    if (!lcm.good())
        return 1;

    Handler handlerObject;
    lcm.subscribe("IA", &Handler::handleMessage, &handlerObject);
    lcm.subscribe("tartarus", &Handler::handletartarus, &handlerObject);

    while (true) {
        std::cout << "Waiting for LCM message..." << std::endl;
        lcm.handle();
        
    }
    close(sock);
    return 0;
}
