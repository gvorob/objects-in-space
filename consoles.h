#ifndef CONSOLES_H
#define CONSOLES_H

#include "main_structs.h"
#include "util.h"

#define INSTRUCTIONS_STRING "Press e to confirm; q to go back; w/a/s/d move around; space to exit"


#define WEAPONS_MAX_WEAP 2
#define WEAPONS_MAX_WEAP_STRING 20

#define WEAPONS_AIMING_INSTRUCTIONS_STRING "Use w/a/s/d to change where you are targeting; q to change weapons"
#define WEAPONS_STATUS_INSTRUCTIONS_STRING "Use e to fire, q to change weapons"

typedef enum _weapons_console_type{
	WCT_AIM,
	WCT_STAT,
} weapons_console_type;

typedef struct _weapons_console_state_struct {
	float target_xs[WEAPONS_MAX_WEAP];
	float target_ys[WEAPONS_MAX_WEAP];
	weapon_type types[WEAPONS_MAX_WEAP]; 
	int current_weapon;
	double weapon_charges[WEAPONS_MAX_WEAP];
}  weapons_console_state_struct;


#define ENEMY_COORD_UPDATE_FRAMES FPS //updates once per frame
#define ENEMY_COORD_UPDATE_DELAY 5
#define SENSORS_MAIN_MENU_OPTIONS 3
typedef enum _sensors_menu_state {
	SMS_SHIP,
	SMS_ENEMY,
	SMS_OTHER,
	SMS_MAIN,
} sensors_menu_state;

typedef struct _sensors_console_state_struct {
	sensors_menu_state current_menu;
	int selected;
	int enemy_coord_frame_counter;//only updates once a second
	
	//New ones are put into the highest position, then are marched down and read from [0]
	float delayed_enemy_x_coords[ENEMY_COORD_UPDATE_DELAY];
	float delayed_enemy_y_coords[ENEMY_COORD_UPDATE_DELAY];
}  sensors_console_state_struct;

typedef struct _repairs_console_state_struct {

}  repairs_console_state_struct;

#define FTL_LEFT_MARGIN 3
#define FTL_MAX_DESTS 3
#define FTL_MAX_DEST_STRING 30
#define FTL_DESTS_TOP 5
#define FTL_CHARGE_BAR_WIDTH ((CONSOLE_PANEL_WIDTH) - 2 * (FTL_CHARGE_BAR_LEFT))
#define FTL_CHARGE_BAR_TOP ((CONSOLE_PANEL_HEIGHT) - 3)
#define FTL_CHARGE_BAR_LEFT (FTL_LEFT_MARGIN)
typedef struct _ftl_console_state_struct {
	char destinations[FTL_MAX_DESTS][FTL_MAX_DEST_STRING];
	int current;
}  ftl_console_state_struct;

#define ENGINES_LEFT_MARGIN 3
#define ENGINES_MAX_STATES 3
//ALWAYS 1 MORE THAN STATES
#define ENGINES_MAX_DEST_STRING 30
#define ENGINES_DESTS_TOP 5
#define ENGINES_CHARGE_BAR_WIDTH ((CONSOLE_PANEL_WIDTH) - 2 * (FTL_CHARGE_BAR_LEFT))
#define ENGINES_CHARGE_BAR_TOP ((CONSOLE_PANEL_HEIGHT) - 4)
#define ENGINES_CHARGE_BAR_LEFT (FTL_LEFT_MARGIN)
#define ENGINES_EVADE_BOX_TOP 5
#define ENGINES_EVADE_BOX_LEFT 45
#define ENGINES_CURR_FLIGHT_STATE_TOP 12
#define ENGINES_SHOT_INFO_TOP 14
#define ENGINES_EVADING_STATUS_TOP 16

#define ENGINES_EVADE_HEAT_COST 0.3f //fraction of the heat bar
#define ENGINES_EVADE_DURATION 1.5f
#define ENGINES_COOLDOWN_TIME 20.0f

typedef struct _engines_console_state_struct {
  int current;
  int evade_selected;
  char states[ENGINES_MAX_STATES][ENGINES_MAX_DEST_STRING];
  float evasive_action; //0-1
} engines_console_state_struct;

void init_weapons_console(
	gamestate_struct*);
void render_weapons_console(
	int client_index, 
	int metadata,
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_weapons_console(
	int client_index, 
	int metadata,
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);
void update_weapons_console(
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);


void init_sensors_console(
	gamestate_struct*);
void render_sensors_console(
	int client_index, 
	int metadata,
	sensors_console_state_struct* scss,
	gamestate_struct* gs);
void update_input_sensors_console(
	int client_index, 
	int metadata,
	sensors_console_state_struct* scss,
	gamestate_struct* gs);
void update_sensors_console(
	sensors_console_state_struct* scss,
	gamestate_struct* gs);

void init_engines_console(
	gamestate_struct*);
void render_engines_console(
	int client_index, 
	int metadata,
	engines_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_engines_console(
	int client_index, 
	int metadata,
	engines_console_state_struct* wcss,
	gamestate_struct* gs);
void update_engines_console(
	engines_console_state_struct* wcss,
	gamestate_struct* gs);


void init_repairs_console(
	gamestate_struct*);
void render_repairs_console(
	int client_index, 
	int metadata,
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_repairs_console(
	int client_index, 
	int metadata,
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);
void update_repairs_console(
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);


void init_ftl_console(
	gamestate_struct*);
void render_ftl_console(
	int client_index, 
	int metadata,
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_ftl_console(
	int client_index, 
	int metadata,
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);
void update_ftl_console(
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);

#endif
