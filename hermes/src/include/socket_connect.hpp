#ifndef SOCKET_CONNECT_HPP
#define SOCKET_CONNECT_HPP

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT_GRSIM 20011
#define IP_GRSIM "127.0.0.1"

extern int sock_grsim;
extern struct sockaddr_in addr_grsim;

void setupSocket_grsim();

#endif