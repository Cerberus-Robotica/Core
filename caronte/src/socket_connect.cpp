#include "include/socket_connect.hpp"
#include "include/tartarus.hpp"

// tempo de timeout em {1,..}segundos e {..., 0}microssegundos
struct timeval tv = {1, 0};

int sock_GC, sock_vision, sock_TRACKED;
struct sockaddr_in addr_GC, addr_vision, addr_TRACKED;
char buffer_GC[BUFFER_SIZE];
char buffer_vision[BUFFER_SIZE];
char buffer_TRACKED[BUFFER_SIZE];

void setupGCSocket() {
    int reuse = 1;
    // Criação do socket para GC
    sock_GC = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Permitir múltiplas conexões no mesmo socket
    
    setsockopt(sock_GC, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    if (setsockopt(sock_GC, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Erro ao configurar timeout do socket Vision");
        close(sock_GC);
        return;
    }

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

    if (setsockopt(sock_vision, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Erro ao configurar timeout do socket Vision");
        close(sock_vision);
        return;
    }

    // Bind do socket na porta multicast
    memset(&addr_vision, 0, sizeof(addr_vision));
    addr_vision.sin_family = AF_INET;
    addr_vision.sin_addr.s_addr = htonl(INADDR_ANY);
    han.new_tartarus.ssl_vision == 1 
        ? addr_vision.sin_port = htons(MCAST_PORT_VISION_SSLVISION)
        : addr_vision.sin_port = htons(MCAST_PORT_VISION_GRSIM);

    bind(sock_vision, (struct sockaddr*)&addr_vision, sizeof(addr_vision));


    // Configuração do grupo multicast para Vision
    struct ip_mreq mreq_vision;
    mreq_vision.imr_multiaddr.s_addr = inet_addr(MCAST_GRP_VISION);
    mreq_vision.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(sock_vision, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq_vision, sizeof(mreq_vision));
}

void setupTrackedSocket() {
    int reuse = 1;

    sock_TRACKED = socket(AF_INET, SOCK_DGRAM, 0);
    
    setsockopt(sock_TRACKED, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));

    if (setsockopt(sock_TRACKED, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Erro ao configurar timeout do socket Vision");
        close(sock_TRACKED);
        return;
    }

    memset((char*)&addr_TRACKED, 0, sizeof(addr_TRACKED));
    addr_TRACKED.sin_family = AF_INET;
    addr_TRACKED.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_TRACKED.sin_port = htons(MCAST_PORT_VISION_TRACKED);

    if (bind(sock_TRACKED, (struct sockaddr*)&addr_TRACKED, sizeof(addr_TRACKED)) < 0) {
        perror("Erro no bind do socket TRACKED");
        exit(1);
    }

    struct ip_mreq mreq_tracked;
    mreq_tracked.imr_multiaddr.s_addr = inet_addr(MCAST_GRP_VISION);
    mreq_tracked.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock_TRACKED, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq_tracked, sizeof(mreq_tracked)) < 0) {
        perror("Erro ao entrar no grupo multicast TRACKED");
        exit(1);
    }
}
