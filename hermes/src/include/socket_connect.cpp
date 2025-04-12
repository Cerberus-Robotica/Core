#include "socket_connect.hpp"

int sock_grsim;
struct sockaddr_in addr_grsim;

void setupSocket_grsim() {

    memset(&addr_grsim, 0, sizeof(addr_grsim));
    addr_grsim.sin_family = AF_INET;
    addr_grsim.sin_port = htons(PORT_GRSIM);

    unsigned char ttl = 2;

    inet_pton(AF_INET, IP_GRSIM, &addr_grsim.sin_addr);
    setsockopt(sock_grsim, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
}
