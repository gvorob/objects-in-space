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
	warnx("init_ftl_console not yet implemented");
};

void render_ftl_console(
		int client_index, 
		ftl_console_state_struct* wcss,
		gamestate_struct* gs) {

	char* rp;
	rp = (gs->clients[client_index].render.render_data);
  
	char message_string[] = "FTL Drive";

	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT + 2, CONSOLE_PANEL_TOP), message_string, 99);
}

void update_input_ftl_console(
		int client_index, 
		ftl_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_input_ftl_console not yet implemented");
}

void update_ftl_console(
		ftl_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_ftl_console not yet implemented");
}
//==========================



