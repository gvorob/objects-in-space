#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "consoles.h"
#include "util.h"
#include "encounter.h"

void update_input_main_game(int client_index, gamestate_struct* gs);
void update_main_game(gamestate_struct* gs);
void render_main_game(int client_index, gamestate_struct* gs);
void setup_game(gamestate_struct* gs);


#endif
