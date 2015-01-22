#ifndef CLIENT_H
#define CLIENT_H

#include "util.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>

void init_client(char* ip, char* port);
int connect_to(char* ip, char* port);
//setup_ncurses(???);
void client_main_loop(int socket_d);
void send_input();
void get_render();

#endif
