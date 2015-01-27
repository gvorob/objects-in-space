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
	shot_struct *ssp, *temp;
	for(ssp = &(gs->encounter.shots_list); ssp->next != NULL; ) {
		temp = ssp->next->next;
		free(ssp->next);
		ssp->next = temp;
	}
}

void update_shots(gamestate_struct* gs) {
	//=======================
	//Update Shots
		//walk the LL
	shot_struct *ssp;
	for(ssp = &(gs->encounter.shots_list); ssp->next != NULL; ) {
		ssp->next->time_to_fly--;

		//if hit
		if(!ssp->next->time_to_fly) {
			shot_struct s = *(ssp->next);
			
			if(gs->shipstate.evasive_action <= 0.0f) {
				//deal damage
				gs->shipstate.health--;
				effect_hit(s.target_x, s.target_y, gs);
			} else {
				effect_miss(s.target_x, s.target_y, s.target_x - s.entry_x, s.target_y - s.entry_y, gs);
				//miss
				
			}
			//delete the shot
			ssp->next = ssp->next->next;

		} else {
			//Walk the list manually so that we can delete things as we go
			ssp = ssp->next;
		}
	}
}

void render_shots(int client_index, gamestate_struct* gs){
	char* rp; //render pointer
	shot_struct *ssp;

	rp = (gs->clients[client_index].render.render_data);

	//walk the LL
	for(ssp = &(gs->encounter.shots_list); ssp->next != NULL; ssp = ssp->next) {
		int temp_x, temp_y;
		shot_struct *temp_ssp = ssp->next;

		//If shot is onscreen
		if(temp_ssp->time_to_fly < temp_ssp->entry_time) {
			//goes from 1 -> 0 as shot approaches target from edge of screen
			float rev_percent_of_trip = ((float)temp_ssp->time_to_fly / (float)temp_ssp->entry_time);

			temp_x = temp_ssp->target_x + 
					(int)(rev_percent_of_trip * (temp_ssp->entry_x - temp_ssp->target_x));
			temp_y = temp_ssp->target_y + 
					(int)(rev_percent_of_trip * (temp_ssp->entry_y - temp_ssp->target_y));

			if(temp_x != clamp(temp_x, 0, SCREEN_WIDTH))
				err(-1, "WOOPX %d %f", temp_x, rev_percent_of_trip);
			if(temp_y != clamp(temp_y, 0, SCREEN_HEIGHT))
				err(-1, "WOOPY %d %f", temp_y, rev_percent_of_trip);

			rp[SCREEN_INDEX(temp_x, temp_y)] = '+';
		}
	}
}

void encounter_reset_fire_delay(gamestate_struct *gs) {
	gs->encounter.fire_delay = rand_int(
			secs_to_frames(ENC_MIN_FIRE_DELAY), 
			secs_to_frames(ENC_MAX_FIRE_DELAY));
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

void encounter_try_firing(gamestate_struct *gs) {
	//if we're ready to fire, do so
	if(gs->encounter.fire_delay == 0) {
		encounter_fire(gs);
		encounter_reset_fire_delay(gs);
	} else {
		gs->encounter.fire_delay--;
	}
}

void encounter_fire(gamestate_struct *gs) {
	shot_struct temp_ss;

	//init shot
	temp_ss.type = ST_ENEMY;

	//Targeting
	pick_target_on_ship(&(temp_ss.target_x), &(temp_ss.target_y), gs);
	temp_ss.time_to_fly = secs_to_frames(ENC_TIME_TO_FLY);

	//Pick random entry point
	switch(rand_int(0,3)) {
		case 0:
			temp_ss.entry_x = rand_int(0, gs->shipstate.tiles.width - 1);
			temp_ss.entry_y = 0;
			break;
		case 1:
			temp_ss.entry_x = rand_int(0, gs->shipstate.tiles.width - 1);
			temp_ss.entry_y = gs->shipstate.tiles.height - 1;
			break;
		case 2:
			temp_ss.entry_x = 0;
			temp_ss.entry_y = rand_int(0, gs->shipstate.tiles.height - 1);
			break;
		case 3:
			temp_ss.entry_x = gs->shipstate.tiles.width - 1;
			temp_ss.entry_y = rand_int(0, gs->shipstate.tiles.height - 1);
			break;
		default:
			err(-1, "Something went horribly wrong with rand_int in encounter_fire()");
	}

	//Figure out timing, after this it's just linear
	float dx, dy;
	float dist;
	int visible_time;
	dx = temp_ss.target_x - temp_ss.entry_x;
	dy = temp_ss.target_y - temp_ss.entry_y;
	dist = euclid_dist(dx, dy);
	visible_time = secs_to_frames(dist / ENC_SHOT_MOVE_SPEED);
	
	temp_ss.entry_time = visible_time;

	//create and add to LL
	create_shot(&temp_ss, gs);
}

void create_shot(shot_struct *ss, gamestate_struct *gs) {
	shot_struct *new_ss;
	
	//malloc a new struct
	new_ss = (shot_struct *) malloc(sizeof(shot_struct));

	//copy over data
	memcpy(new_ss, ss, sizeof(shot_struct));

	//Insert into linked list
	new_ss->next = gs->encounter.shots_list.next;
	gs->encounter.shots_list.next = new_ss;

}

void pick_target_on_ship(int *result_x, int *result_y, gamestate_struct *gs) {
	static int num_ship_tiles = -1;
	int i, j, count;
	ship_tiles_struct *stsp;

	stsp = &(gs->shipstate.tiles);

	//Count how many tiles there are the first time around
	if(num_ship_tiles == -1) {
		count = 0;
		for(i = 0; i < stsp->width; i++) {
			for(j = 0; j < stsp->height; j++) {
				if(stsp->tiles_ptr[SHIP_TILES_INDEX(i, j, stsp)].type != TT_SPACE)
					count++;
			}
		}
		num_ship_tiles = count;
	}

	
	//Actual picking happens here
	count = rand_int(0, num_ship_tiles - 1);
	for(i = 0; i < stsp->width; i++) {
		for(j = 0; j < stsp->height; j++) {
			if(stsp->tiles_ptr[SHIP_TILES_INDEX(i, j, stsp)].type != TT_SPACE) {
				//if it's the count'th tile, return it's coords
				if(!count) {
					*result_x = i;
					*result_y = j;
					return;
				}
				
				//otherwise keep looking
				count--;
			}
		}
	}
}
