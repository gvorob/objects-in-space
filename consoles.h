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

void render_weapons_console(
	client_struct* c, 
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_weapons_console(
	client_struct* c, 
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);
void update_weapons_console(
	client_struct* c, 
	weapons_console_state_struct* wcss,
	gamestate_struct* gs);


void render_sensors_console(
	client_struct* c, 
	sensors_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_sensors_console(
	client_struct* c, 
	sensors_console_state_struct* wcss,
	gamestate_struct* gs);
void update_sensors_console(
	client_struct* c, 
	sensors_console_state_struct* wcss,
	gamestate_struct* gs);


void render_engines_console(
	client_struct* c, 
	engines_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_engines_console(
	client_struct* c, 
	engines_console_state_struct* wcss,
	gamestate_struct* gs);
void update_engines_console(
	client_struct* c, 
	engines_console_state_struct* wcss,
	gamestate_struct* gs);


void render_repairs_console(
	client_struct* c, 
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_repairs_console(
	client_struct* c, 
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);
void update_repairs_console(
	client_struct* c, 
	repairs_console_state_struct* wcss,
	gamestate_struct* gs);


void render_ftl_console(
	client_struct* c, 
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);
void update_input_ftl_console(
	client_struct* c, 
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);
void update_ftl_console(
	client_struct* c, 
	ftl_console_state_struct* wcss,
	gamestate_struct* gs);

#endif
