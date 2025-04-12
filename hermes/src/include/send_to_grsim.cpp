#include "send_to_grsim.hpp"

grSim_Commands *ia_commands = &han.commands;
grSim_Commands commands;
grSim_Packet packet_grsim;

void send_to_grsim()
{
    packet_grsim.mutable_commands()->CopyFrom(commands);

    std::cout << "commands size " << packet_grsim.commands().robot_commands_size() << std::endl;
    std::cout << "byte size " << packet_grsim.ByteSize() << std::endl << std::endl;
    packet_grsim.SerializeAsString();

    std::string serialized_packet = packet_grsim.SerializeAsString();
    sendto(sock_grsim, serialized_packet.c_str(), serialized_packet.size(), 0, (struct sockaddr*) &addr_grsim, sizeof(addr_grsim));
}