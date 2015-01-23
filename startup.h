#ifndef STARTUP_H
#define STARTUP_H

#include "util.h"
#include "main_structs.h"
#include "socket_util.h"

void init_server(gamestate_struct* gs);
int setup_listening_socket(char* port);
void setup_connections_lobby(gamestate_struct* gs);
void update_input_connecting(int client_index, gamestate_struct* gs);
void update_connecting(gamestate_struct* gs);
void render_connecting(int client_index, gamestate_struct* gs);
void finalize_connections(gamestate_struct* gs);
void setup_game(gamestate_struct* gs);

#endif
