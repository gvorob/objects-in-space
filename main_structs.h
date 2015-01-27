#ifndef MAIN_STRUCTS_H
#define MAIN_STRUCTS_H

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
#define CONFIRM 'e'
#define CANCEL 'q'
#define CONSOLE_LOCK ' '

#include "util.h"
//for defines

typedef struct _client_input_struct {
  int left;
  int right;
  int up;
  int down;
  int confirm;
  int cancel;
  int console_lock;
  //more TBD
} client_input_struct;

typedef struct _client_render_struct {
  char render_data[SCREEN_WIDTH * SCREEN_HEIGHT];
} client_render_struct;

#define SCREEN_INDEX(x,y) ((x) + (y) * (SCREEN_WIDTH))

typedef struct _client_struct { //DONE
	client_input_struct prev_input_state;
	client_input_struct curr_input_state;
	int socket_d; 
	//If client not connected, socket_d == -1;
	client_render_struct render;
} client_struct;


typedef struct _player_struct {
	int x;
	int y;
	int is_at_console;
	int is_connected;
} player_struct;


typedef enum _tile_type {
	TT_SPACE,
	TT_FLOOR,
	TT_WALL,
	TT_ALT_WALL,
	TT_WEAPONS_CONSOLE,
	TT_SENSORS_CONSOLE,
	TT_ENGINES_CONSOLE,
	TT_REPAIRS_CONSOLE,
	TT_FTL_CONSOLE,
	TT_DOOR,
	//???
} tile_type;

typedef struct _tile_struct {
	tile_type type;
	int metadata;
	void* console_state_ptr;
} tile_struct;

typedef struct _ship_tiles_struct {
	int width;
  	int height;
  	tile_struct* tiles_ptr;
} ship_tiles_struct;
#define SHIP_TILES_INDEX(x, y, stsp) ((x) + (y) * (stsp)->width)

typedef enum _console_index {
	CI_WEAPONS,
	CI_SENSORS,
	CI_ENGINES,
	CI_FTL,
} console_index;

typedef enum _flight_state {
  	FS_PASSIVE,
	FS_CHASING,
	FS_STABLE
} flight_state;

typedef enum _weapon_type {
	//lasers,missiles,flak,plasma,etc...
  	WT_LASER,
	WT_MISSILE,
	WT_FLAK,
	WT_PLASMA
} weapon_type;

typedef struct _weapon_struct {
	weapon_type type;
	int time_to_charge;
} weapon_struct;

typedef struct _shipstate_struct {
	//all console states
	//e.g.:
	void *console_states[MAX_CONSOLES];
 	/*
	  malloc a bunch of <...>_console_state_struct-s and fill it in
	  null pointers would be ignored
	*/
	//tiles would point to these with void *’s
	//which we then cast to the right type
	//based on what the console type is (also from the tile)
  
  	ship_tiles_struct tiles;
  	flight_state curr_flight_state;
  	float engine_heat; //0-1
  	weapon_struct weapons[2];
  	float aim_x, aim_y;
  	int health; //0-20
  	float ftl_charge; //0-1
} shipstate_struct;

typedef enum _flow_state {
	FS_MAIN_GAME,
	FS_CONNECTING
} flow_state;

typedef enum _shot_type{
	ST_ENEMY,
	ST_ASTEROID,
} shot_type;

typedef struct _shot_struct {
	int target_x;
	int target_y;
	shot_type type;
	int time_to_fly;
  	struct _shot_struct *next;
} shot_struct;

typedef struct _encounter_struct {
  	float enemy_location_x;
	float enemy_location_y;
  	float target_x;
	float target_y;
	int loiter_time;
	int fire_delay;
	shot_struct shots_list;

  	//firing window stuff
  	int time_to_firing_window;
  	int firing_window_duration;

  	//WHICH FLIGHT STATE SHOULD WE BE IN
  	flight_state firing_window_requirement; 

	int time_to_asteroid;
  	int enemy_max_health;
  	int enemy_health;
} encounter_struct;

//MAX_PLAYERS defined in util.h
typedef struct _gamestate_struct { //NOT DONE YET
	flow_state curr_flow_state;//main game, connecting, dialogue box
	shipstate_struct shipstate;
	player_struct players[MAX_PLAYERS];
	//if players[i].is_connected == 0, not connected
	client_struct clients[MAX_PLAYERS];
	//if clients[i].socket_d == -1, not connected
  	encounter_struct encounter;
} gamestate_struct;

//tile checks
int is_console(tile_type t);
int is_walkable(tile_type t);

#endif
