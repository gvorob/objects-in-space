#ifndef SOCKET_UTIL_H
#define SOCKET_UTIL_H

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>

#define HANDSHAKE_SIZE 64

void *get_in_addr(struct sockaddr *sa);

#endif
