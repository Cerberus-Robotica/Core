#ifndef SOCKET_CONNECT_HPP
#define SOCKET_CONNECT_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#define BUFFER_SIZE 65536

#define MCAST_GRP_GC "224.5.23.1"
#define MCAST_PORT_GC 10003

#define MCAST_GRP_VISION "224.5.23.2"
#define MCAST_PORT_VISION_GRSIM 10020
#define MCAST_PORT_VISION_SSLVISION 10006
#define MCAST_PORT_VISION_TRACKED 10010

extern int sock_GC, sock_vision, sock_TRACKED;
extern struct sockaddr_in addr_GC, addr_vision, addr_TRACKED;
extern char buffer_GC[BUFFER_SIZE];
extern char buffer_vision[BUFFER_SIZE];
extern char buffer_TRACKED[BUFFER_SIZE];

void setupVisionSocket();
void setupGCSocket();
void setupTrackedSocket();

#endif