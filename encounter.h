#include "main_structs.h"
#include "util.h"

//ENCOUNTER PARAMS
#define ENC_MIN_HEALTH 5
#define ENC_MAX_HEALTH 15

#define ENC_MIN_FIRE_DELAY 10.0f //seconds
#define ENC_MAX_FIRE_DELAY 20.0f

#define ENC_MIN_LOC -5.0f
#define ENC_MAX_LOC 5.0f

#define ENC_MIN_LOITER 3.0f //seconds
#define ENC_MAX_LOITER 10.0f

#define ENC_MOVE_SPEED 0.01f // tiles/frame


void setup_encounter(gamestate_struct *gs);
void cleanup_encounter(gamestate_struct *gs);

void encounter_reset_fire_delay(gamestate_struct *gs);
void encounter_reset_enemy_health(gamestate_struct *gs);
void encounter_reset_loiter_time(gamestate_struct *gs);
void encounter_reset_target_loc(gamestate_struct *gs);

//advances loiter timer or moves towards target
void encounter_move_enemy(gamestate_struct *gs);
