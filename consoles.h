#ifndef CONSOLES_H
#define CONSOLES_H

#include "main_structs.h"
#include "util.h"

typedef struct _weapons_console_state_struct {

}  weapons_console_state_struct;

typedef struct _sensors_console_state_struct {

}  sensors_console_state_struct;

typedef struct _engines_console_state_struct {

}  engines_console_state_struct;

typedef struct _repairs_console_state_struct {

}  repairs_console_state_struct;

typedef struct _ftl_console_state_struct {

}  ftl_console_state_struct;

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
