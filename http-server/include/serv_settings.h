#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zlib.h>


#ifdef __linux__
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#ifndef SERV_SETTINGS_H
#define SERV_SETTINGS_H

#define MAX_BIND_TRIES 10
#define PORT           4221

typedef struct sockaddr_in addr_in;

int set_serv_settings(int server_sock);
struct sockaddr* to_sockaddr(addr_in* addr);
addr_in create_addr_in(uint16_t port);

#endif