#ifndef CLIENT_H
#define CLIENT_H

#include "util.h"

void init_client(char* ip, char* port);
int connect_to(char* ip, char* port);
void setup_ncurses();
void client_main_loop(int socket_d);
void send_input();
void get_render();

#endif
