#include "encounter.h"

void setup_encounter(gamestate_struct* gs) {
	encounter_struct *es;
	es = &(gs->encounter);
	
	encounter_reset_enemy_health(gs);

	es->enemy_location_x = 0;
	es->enemy_location_y = 0;
	es->target_x = 0;
	es->target_y = 0;

	encounter_reset_loiter_time(gs);

	encounter_reset_fire_delay(gs);

	es->shots_list.next = NULL;

	es->time_to_firing_window = 0;
	es->firing_window_duration = 0;
	es->firing_window_requirement = 0;

	es->time_to_asteroid = 0;
}

void cleanup_encounter(gamestate_struct* gs) {
	warnx("cleanup_encounter not yet implemented");
}

void encounter_reset_fire_delay(gamestate_struct *gs) {
	gs->encounter.fire_delay = rand_int(ENC_MIN_FIRE_DELAY, ENC_MAX_FIRE_DELAY);
}

void encounter_reset_enemy_health(gamestate_struct *gs) {
	gs->encounter.enemy_max_health = rand_int(ENC_MIN_HEALTH, ENC_MAX_HEALTH);
	gs->encounter.enemy_health = gs->encounter.enemy_max_health;
}

void encounter_reset_loiter_time(gamestate_struct *gs) {
	gs->encounter.loiter_time = rand_int(
			secs_to_frames(ENC_MIN_LOITER),
			secs_to_frames(ENC_MAX_LOITER));
}

//find a new target
void encounter_reset_target_loc(gamestate_struct *gs) {
	gs->encounter.target_x = rand_float(ENC_MIN_LOC, ENC_MAX_LOC);
	gs->encounter.target_y = rand_float(ENC_MIN_LOC, ENC_MAX_LOC);
}

void encounter_move_enemy(gamestate_struct *gs) {
	encounter_struct *es;
	float dist;
	float dx, dy;
	es = &(gs->encounter);

	dx = es->target_x - es->enemy_location_x;
	dy = es->target_y - es->enemy_location_y;
	dist = euclid_dist(dx, dy);
	
	if(es->loiter_time != -1) { //I'm loitering
		//Is it time to stop?
		if(es->loiter_time == 0) { 
			//yes, stop
			es->loiter_time = -1;
			encounter_reset_target_loc(gs);
		} else { 
			//no, keep waiting
			es->loiter_time--;
		}
	} else { //I'm moving
		if(dist < ENC_MOVE_SPEED) { //Have i reached my target?
			//yes, start loitering
			encounter_reset_loiter_time(gs); 
		} else {
			//no, move towards it
			float scale_factor = ENC_MOVE_SPEED / dist;
			es->enemy_location_x += dx * scale_factor;
			es->enemy_location_y += dy * scale_factor;
		}
	}
}
