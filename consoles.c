#include "consoles.h"

//WEAPONS =================
void init_weapons_console(
		gamestate_struct* gs) {
	warnx("init_weapons_console not yet implemented");
};

void render_weapons_console(
		int client_index, 
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("render_weapons_console not yet implemented");
}

void update_input_weapons_console(
		int client_index, 
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_input_weapons_console not yet implemented");
}

void update_weapons_console(
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_weapons_console not yet implemented");
}
//==========================

//SENSORS =================
void init_sensors_console(
		gamestate_struct* gs) {
	warnx("init_sensors_console not yet implemented");
};

void render_sensors_console(
		int client_index, 
		sensors_console_state_struct* wcss,
		gamestate_struct* gs) {

	char* rp;
	int temp_len;
	rp = (gs->clients[client_index].render.render_data);
  
	int enemy_x = gs->encounter.enemy_location_x;
	int enemy_y = gs->encounter.enemy_location_y;
	char location[128];
	temp_len = sprintf(location, "Enemy ship at coordinates %d, %d", enemy_x, enemy_y);
	memcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), location, temp_len);

	int enemy_health = gs->encounter.enemy_health;
	int enemy_max = gs->encounter.enemy_max_health;
	char health[128];
	temp_len = sprintf(health, "Enemy ship at health %d/%d", enemy_health, enemy_max);
	memcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP + 1), health, temp_len);

}

void update_input_sensors_console(
		int client_index, 
		sensors_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_input_sensors_console not yet implemented");
}

void update_sensors_console(
		sensors_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_sensors_console not yet implemented");
}
//==========================



//ENGINES =================
void init_engines_console(
		gamestate_struct* gs) {
	warnx("init_engines_console not yet implemented");
};

void render_engines_console(
		int client_index, 
		engines_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("render_engines_console not yet implemented");
}

void update_input_engines_console(
		int client_index, 
		engines_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_input_engines_console not yet implemented");
}

void update_engines_console(
		engines_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_engines_console not yet implemented");
}
//==========================



//REPAIRS =================
void init_repairs_console(
		gamestate_struct* gs) {
	warnx("init_repairs_console not yet implemented");
};

void render_repairs_console(
		int client_index, 
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("render_repairs_console not yet implemented");
}

void update_input_repairs_console(
		int client_index, 
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_input_repairs_console not yet implemented");
}

void update_repairs_console(
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_repairs_console not yet implemented");
}
//==========================



//FTL =================
void init_ftl_console(
		gamestate_struct* gs) {
	ftl_console_state_struct* fcss;
	int i;

	//Malloc a struct
	fcss = (ftl_console_state_struct*) malloc(sizeof(ftl_console_state_struct));
	if(fcss == NULL)
		err(-1, "Failed to malloc in init_ftl_console");

	//Init members
	for(i = 0; i < FTL_MAX_DESTS; i++)
		snprintf(fcss->destinations[i], FTL_MAX_DEST_STRING, "Destination #%d", i);
	fcss->current = 0;
	fcss->charge = 0;

	//Add into gamestate
	gs->shipstate.console_states[CI_FTL] = fcss;
};

void render_ftl_console(
		int client_index, 
		ftl_console_state_struct *fcss,
		gamestate_struct *gs) {

	char *rp, *temp_rp;
	//int left_offset;
	int i;
	char message_string[] = "                   FTL Drive";
	char charged_string[] = "                    CHARGED";

	//prepare pointers
	rp = (gs->clients[client_index].render.render_data);
  
	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
			message_string, 
			CONSOLE_PANEL_WIDTH);

	//render charge bar
	int charge_width = (int)((fcss->charge) * (float)FTL_CHARGE_BAR_WIDTH + 0.5);
	temp_rp = (char *)(rp + SCREEN_INDEX(
			FTL_CHARGE_BAR_LEFT + CONSOLE_PANEL_LEFT,
			FTL_CHARGE_BAR_TOP + CONSOLE_PANEL_TOP));
	for(i = 0; i < FTL_CHARGE_BAR_WIDTH; i++) {
		temp_rp[i] = (i > charge_width) ? '-' : '+';
	}
	
	//CHARGED message
	if(fcss->charge >= 1.0f)
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, FTL_CHARGE_BAR_TOP + 1), 
				charged_string, 
				CONSOLE_PANEL_WIDTH);

	//Show destinations
	for(i = 0; i < FTL_MAX_DESTS; i++) {
		render_strcpy(rp + 
				SCREEN_INDEX(
						CONSOLE_PANEL_LEFT + FTL_LEFT_MARGIN, 
						CONSOLE_PANEL_TOP + FTL_DESTS_TOP + i * 2), 
				fcss->destinations[i], 
				CONSOLE_PANEL_WIDTH - FTL_LEFT_MARGIN);
	}

	//Show current destination
	rp[SCREEN_INDEX(
				CONSOLE_PANEL_LEFT + FTL_LEFT_MARGIN - 1, 
				CONSOLE_PANEL_TOP + FTL_DESTS_TOP + fcss->current * 2)] = '>';
}

void update_input_ftl_console(
		int client_index, 
		ftl_console_state_struct* fcss,
		gamestate_struct* gs) {
	
	client_input_struct* cisp;
	cisp = &(gs->clients[client_index].curr_input_state);

	if(cisp->up) {
		fcss->current--;
	}
	if(cisp->down) {
		fcss->current++;
	}

	fcss->current = clamp(fcss->current, 0, FTL_MAX_DESTS - 1);
}

void update_ftl_console(
		ftl_console_state_struct* fcss,
		gamestate_struct* gs) {
	//Has to charge in 20 seconds
	//20 * 30 = 600 frams
	fcss->charge = fclamp(fcss->charge + 1.0f/600.0f, 0, 1);
}
//==========================



