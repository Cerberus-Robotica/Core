#include "socket_connect.hpp"

int sock_grsim, sock_control_grsim;
struct sockaddr_in addr_grsim, addr_control_grsim;


void setupSocket_grsim() {

    sock_grsim = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);    

    memset(&addr_grsim, 0, sizeof(addr_grsim));
    addr_grsim.sin_family = AF_INET;
    addr_grsim.sin_port = htons(PORT_GRSIM);

    unsigned char ttl = 2;

    inet_pton(AF_INET, IP_GRSIM, &addr_grsim.sin_addr);
    setsockopt(sock_grsim, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
}

void setupSocket_control_grsim() {

    sock_control_grsim = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    memset(&addr_control_grsim, 0, sizeof(addr_control_grsim));
    addr_control_grsim.sin_family = AF_INET;
    addr_control_grsim.sin_port = htons(PORT_CONTROL_GRSIM);

    unsigned char ttl = 2;

    inet_pton(AF_INET, IP_GRSIM, &addr_control_grsim.sin_addr);
    setsockopt(sock_control_grsim, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
}
