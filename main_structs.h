#ifndef MAIN_STRUCTS_H
#define MAIN_STRUCTS_H

typedef struct _client_input_struct {

} client_input_struct;

typedef struct _client_render_struct {

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
};

typedef struct _tile_struct {

} tile_struct;

typedef struct _ship_tiles_struct {

} ship_tiles_struct;
#define SHIP_TILES_INDEX(x, y, stsp)

typedef struct _shipstate_struct {

} shipstate_struct;

enum flow_state {
	FS_MAIN_GAME,
	FS_CONNECTING,
};

typedef struct _gamestate_struct {

} gamestate_struct;


#endif
