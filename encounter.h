#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "main_structs.h"
#include "util.h"

//ENCOUNTER PARAMS
#define ENC_MIN_HEALTH 5
#define ENC_MAX_HEALTH 15

#define ENC_MIN_FIRE_DELAY 3.0f //seconds
#define ENC_MAX_FIRE_DELAY 8.0f

#define ENC_MIN_LOC -5.0f
#define ENC_MAX_LOC 5.0f

#define ENC_MIN_LOITER 3.0f //seconds
#define ENC_MAX_LOITER 10.0f

#define ENC_MOVE_SPEED 0.01f // tiles/frame

#define ENC_TIME_TO_FLY 5.0f //seconds
#define ENC_SHOT_MOVE_SPEED 20.0f // tiles/fram

void setup_encounter(gamestate_struct *gs);
void cleanup_encounter(gamestate_struct *gs);

void encounter_reset_fire_delay(gamestate_struct *gs);
void encounter_reset_enemy_health(gamestate_struct *gs);
void encounter_reset_loiter_time(gamestate_struct *gs);
void encounter_reset_target_loc(gamestate_struct *gs);

//advances loiter timer or moves towards target
void encounter_move_enemy(gamestate_struct *gs);

//Ticks down fire timer (or fires if finished)
void encounter_try_firing(gamestate_struct *gs);

//actually fires a shot
void encounter_fire(gamestate_struct *gs);

//Mallocs a shot_struct, copies the values from ss into it, 
//then inserts it into the linked list in gs
void create_shot(shot_struct *ss, gamestate_struct *gs);

//Picks a random tile of the ship, and puts its coords into *result_x and y
void pick_target_on_ship(int *result_x, int *result_y, gamestate_struct *gs);

#endif
