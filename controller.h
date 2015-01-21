#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "main_game.h"
#include "main_structs.h"
#include "startup.h"
#include "client.h"
#include "util.h"

int main(int argc, char *argv[]);
void get_input(client_struct* c);
void update_input(client_struct* c, gamestate_struct* gs);
void update(gamestate_struct* gs);
void render(client_struct* c, gamestate_struct* gs);

#endif
