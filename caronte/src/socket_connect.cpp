#include "include/socket_connect.hpp"


int sock_GC, sock_vision;
struct sockaddr_in addr_GC, addr_vision;
char buffer_GC[BUFFER_SIZE];
char buffer_vision[BUFFER_SIZE];

void setupGCSocket() {
    int reuse = 1;
    // Criação do socket para GC
    sock_GC = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Permitir múltiplas conexões no mesmo socket
    
    setsockopt(sock_GC, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));


    // Bind do socket na porta multicast
    memset(&addr_GC, 0, sizeof(addr_GC));
    addr_GC.sin_family = AF_INET;
    addr_GC.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_GC.sin_port = htons(MCAST_PORT_GC);

    bind(sock_GC, (struct sockaddr*)&addr_GC, sizeof(addr_GC));


    // Configuração do grupo multicast para GC
    struct ip_mreq mreq_GC;
    mreq_GC.imr_multiaddr.s_addr = inet_addr(MCAST_GRP_GC);
    mreq_GC.imr_interface.s_addr = inet_addr("172.17.0.1"); // <-- usar essa linha para receber do GC com docker
    //mreq_GC.imr_interface.s_addr = htonl(INADDR_ANY); // <-- usar essa linha para receber do GC instalado localmente

    setsockopt(sock_GC, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq_GC, sizeof(mreq_GC));
}

void setupVisionSocket() {
    int reuse = 1;
    // Criação do socket para Vision
    sock_vision = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Permitir múltiplas conexões no mesmo socket
    setsockopt(sock_vision, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // Bind do socket na porta multicast
    memset(&addr_vision, 0, sizeof(addr_vision));
    addr_vision.sin_family = AF_INET;
    addr_vision.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_vision.sin_port = htons(MCAST_PORT_VISION);

    bind(sock_vision, (struct sockaddr*)&addr_vision, sizeof(addr_vision));


    // Configuração do grupo multicast para Vision
    struct ip_mreq mreq_vision;
    mreq_vision.imr_multiaddr.s_addr = inet_addr(MCAST_GRP_VISION);
    mreq_vision.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(sock_vision, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq_vision, sizeof(mreq_vision));
}