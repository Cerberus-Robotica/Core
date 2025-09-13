#ifndef SOCKET_CONNECT_HPP
#define SOCKET_CONNECT_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT_GRSIM 20011
#define PORT_CONTROL_GRSIM 10300
#define IP_GRSIM "127.0.0.1"

extern int sock_grsim, sock_control_grsim;
extern struct sockaddr_in addr_grsim, addr_control_grsim;

extern void setupSocket_grsim();
extern void setupSocket_control_grsim();

#endif