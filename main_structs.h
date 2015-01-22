#ifndef MAIN_STRUCTS_H
#define MAIN_STRUCTS_H

#define LEFT 'a'
#define RIGHT 'd'
#define UP 'w'
#define DOWN 's'
#define CONSOLE_LOCK ' '
typedef struct _client_input_struct {
  int left;
  int right;
  int up;
  int down;
  int console_lock;
  //more TBD
} client_input_struct;

//SCREEN WIDTH/HEIGHT TBD
#define SCREEN_WIDTH 10
#define SCREEN_HEIGHT 10
typedef struct _client_render_struct {
  char render_data[SCREEN_WIDTH * SCREEN_HEIGHT];
} client_render_struct;

#define SCREEN_INDEX(x,y)

typedef struct _client_struct {

} client_struct;


typedef struct _player_struct {

} player_struct;


enum tile_type {
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
  enum tile_type type;
  void* console_state_ptr;
} tile_struct;

typedef struct _ship_tiles_struct {
  int width;
  int height;
  tile_struct* tiles_ptr;
} ship_tiles_struct;
#define SHIP_TILES_INDEX(x, y, stsp)

//MAX CONSOLES TBD
#define MAX_CONSOLES 2
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
} shipstate_struct;

enum flow_state {
	FS_MAIN_GAME,
	FS_CONNECTING,
} flow_state;

//MAX PLAYERS TBD
#define MAX_PLAYERS 3
typedef struct _gamestate_struct {
  enum flow_state curr_flow_state;//main game, connecting, dialogue box
  shipstate_struct shipstate;
  player_struct players[MAX_PLAYERS];
  //if players[i].is_connected == 0, not connected
  client_struct clients[MAX_PLAYERS];
//if clients[i].socket_d == -1, not connected

} gamestate_struct;


#endif
