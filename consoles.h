#ifndef CONSOLES_H
#define CONSOLES_H

#include "main_structs.h"
#include "util.h"

#define WEAPONS_MAX_WEAP 2
#define WEAPONS_MAX_WEAP_STRING 20
typedef struct _weapons_console_state_struct {
	int targx;
	int targy;
	char weapon_names[WEAPONS_MAX_WEAP][WEAPONS_MAX_WEAP_STRING];
	double weapon_charges[WEAPONS_MAX_WEAP];
}  weapons_console_state_struct;

typedef struct _sensors_console_state_struct {

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
	float charge;
}  ftl_console_state_struct;

#define ENGINES_MAX_STATES 3
typedef struct _engines_console_state_struct {
  float engine_heat; //pushes info to shipstate, 0-1
  flight_state curr_flight_state; //pushes info to shipstate
  int current;
  char states[ENGINES_MAX_STATES][FTL_MAX_DEST_STRING];
} engines_console_state_struct;

void init_weapons_console(
	gamestate_struct*);
void render_weapons_console(
	int client_index, 
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_weapons_console(
	int client_index, 
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);
void update_weapons_console(
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);


void init_sensors_console(
	gamestate_struct*);
void render_sensors_console(
	int client_index, 
	sensors_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_sensors_console(
	int client_index, 
	sensors_console_state_struct* wcss,
	gamestate_struct* gs);
void update_sensors_console(
	sensors_console_state_struct* wcss,
	gamestate_struct* gs);

void init_engines_console(
	gamestate_struct*);
void render_engines_console(
	int client_index, 
	engines_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_engines_console(
	int client_index, 
	engines_console_state_struct* wcss,
	gamestate_struct* gs);
void update_engines_console(
	engines_console_state_struct* wcss,
	gamestate_struct* gs);


void init_repairs_console(
	gamestate_struct*);
void render_repairs_console(
	int client_index, 
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_repairs_console(
	int client_index, 
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);
void update_repairs_console(
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);


void init_ftl_console(
	gamestate_struct*);
void render_ftl_console(
	int client_index, 
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_ftl_console(
	int client_index, 
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);
void update_ftl_console(
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);

#endif
