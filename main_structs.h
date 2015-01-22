#ifndef MAIN_STRUCTS_H
#define MAIN_STRUCTS_H

#include "util.h"
//for defines

typedef struct _client_input_struct {

} client_input_struct;

typedef struct _client_render_struct {

} client_render_struct;

#define SCREEN_INDEX(x,y)

typedef struct _client_struct { //DONE
	client_input_struct prev_input_state;
	client_input_struct curr_input_state;
	int socket_d;
	//If client not connected, socket_d == -1;
	client_render_struct render;
} client_struct;


typedef struct _player_struct {

} player_struct;


typedef enum _tile_type {
	TT_FLOOR,
	TT_WALL,
	TT_WEAPONS_CONSOLE,
	TT_SENSORS_CONSOLE,
	TT_ENGINES_CONSOLE,
	TT_REPAIRS_CONSOLE,
	TT_FTL_CONSOLE,
	TT_DOOR,
	//???
} tile_type;

typedef struct _tile_struct {

} tile_struct;

typedef struct _ship_tiles_struct {

} ship_tiles_struct;
#define SHIP_TILES_INDEX(x, y, stsp)

typedef struct _shipstate_struct {

} shipstate_struct;

typedef enum _flow_state {
	FS_MAIN_GAME,
	FS_CONNECTING
} flow_state;

//MAX_PLAYERS defined in util.h
typedef struct _gamestate_struct { //NOT DONE YET
	flow_state curr_flow_state;//main game, connecting, dialogue box
	shipstate_struct shipstate;
	player_struct players[MAX_PLAYERS];
	//if players[i].is_connected == 0, not connected
	client_struct clients[MAX_PLAYERS];
	//if clients[i].socket_d == -1, not connected

} gamestate_struct;


#endif
