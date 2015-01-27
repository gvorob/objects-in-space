#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "consoles.h"
#include "util.h"
#include "encounter.h"

#define SHIP_MAX_HEALTH 20
#define SHIP_FTL_CHARGE_TIME 20.0f //seconds


void update_input_main_game(int client_index, gamestate_struct* gs);
void update_main_game(gamestate_struct* gs);
void render_main_game(int client_index, gamestate_struct* gs);
void setup_game(gamestate_struct* gs);


#endif
