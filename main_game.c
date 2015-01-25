#include "main_game.h"

void update_input_main_game(int client_index, gamestate_struct* gs) {
	client_input_struct* cisp;
	player_struct* psp;
	ship_tiles_struct* stsp;

	warnx("updating input for player %d", client_index);
	
	cisp = &(gs->clients[client_index].curr_input_state);
	psp = &(gs->players[client_index]);

	if(psp->is_at_console){
		///////////////////////////////
	 	//NOT DONE YET
	  	ship_tiles_struct tiles = gs->shipstate->tiles;
	  	tile_struct player_tile = tiles->tiles_ptr[SHIP_TILES_INDEX(psp->x, psp->y, tiles)];
	  	//////////////////////////////
	}

	stsp = &(gs->shipstate.tiles);

	if(cisp->console_lock) {
		//If standing on a console
		tile_struct temp_tile;
		temp_tile = stsp->tiles_ptr[SHIP_TILES_INDEX(psp->x, psp->y, stsp)];
		if(is_console(temp_tile.type)) {
			psp->is_at_console = !(psp->is_at_console);
		}
	}

	//Walk around
	if(!psp->is_at_console) {
		int tempx, tempy;
		tile_struct temp_tile;

		tempx = psp->x;
		tempy = psp->y;

		if(cisp->up) {
			tempy--;
		}
		if(cisp->down) {
			tempy++;
		}
		if(cisp->left) {
			tempx--;
		}
		if(cisp->right) {
			tempx++;
		}

		tempx = clamp(tempx, 0, LOBBY_WIDTH - 1); 
		tempy = clamp(tempy, 0, LOBBY_HEIGHT - 1); 

		temp_tile = stsp->tiles_ptr[SHIP_TILES_INDEX(tempx, tempy, stsp)];

		if(is_walkable(temp_tile.type)) {
			psp->x = tempx;
			psp->y = tempy;
		}
	}
}

void update_main_game(gamestate_struct* gs) {
	warnx("update_main_game not yet implemented");
}

void render_main_game(int client_index, gamestate_struct* gs) {
	char* rp; //render pointer
	ship_tiles_struct* stsp;
	player_struct ps;
	int i, j;
	int total_players = 0;

	//Prepare pointers
	rp = (gs->clients[client_index].render.render_data);
	stsp = &(gs->shipstate.tiles);


	//Render Ship
	for(i = 0; i < stsp->width; i++) {
		for(j = 0; j < stsp->height; j++) {
			tile_struct temp_ts;
			temp_ts = stsp->tiles_ptr[SHIP_TILES_INDEX(i, j, stsp)];

			switch(temp_ts.type) {
				case TT_SPACE:
					rp[SCREEN_INDEX(i,j)] = ' ';
					break;
				case TT_FLOOR:
					rp[SCREEN_INDEX(i,j)] = '.';
					break;
				case TT_WEAPONS_CONSOLE:
				case TT_SENSORS_CONSOLE:
				case TT_ENGINES_CONSOLE:
				case TT_REPAIRS_CONSOLE:
				case TT_FTL_CONSOLE:
					rp[SCREEN_INDEX(i,j)] = 'C'; //temporary, just marks as console
					break;
				case TT_WALL:
					rp[SCREEN_INDEX(i,j)] = '#';
					break;
				case TT_ALT_WALL:
					rp[SCREEN_INDEX(i,j)] = 'X';
					break;
				default:
					rp[SCREEN_INDEX(i,j)] = '?';
					break;
			}
		}
	}

	//Draw in players
	for(i = 0; i < MAX_PLAYERS; i++) {
		ps = gs->players[i];
		if(ps.is_connected) {
			rp[SCREEN_INDEX(ps.x, ps.y)] = '@';
			total_players++;
		}
	}

	//DEBUGGING
	ps = gs->players[client_index];
	sprintf(rp + SCREEN_INDEX(0, stsp->height), "is_at_console:%d", ps.is_at_console);

}

/*
	prepare gamestate, shipstate, load in ships, encounter, etc
	note: gamestate should be partly preloaded from setup_connections_lobby 
*/
void setup_game(gamestate_struct* gs){
	int width, height;
	int i, j, k;
	ship_tiles_struct* stsp;
	char* temp_buff;

	stsp = &(gs->shipstate.tiles);

	//free existing tile array
	free(stsp->tiles_ptr);
	stsp->tiles_ptr = NULL;

	//prepare consoles
	for(i = 0; i < MAX_CONSOLES; i++)
		gs->shipstate.console_states[i] = NULL;
	init_weapons_console(gs);
	init_engines_console(gs);
	init_sensors_console(gs);
	init_ftl_console(gs);

	//start reading in ship layout
	FILE* file = fopen(SHIP_FILE, "r");
	if(file == NULL)
		err(-1, "Failed to open ship file \"%s\"", SHIP_FILE);

	//Prepare new tile_struct matrix
	fscanf(file, "%d %d\n", &width, &height);
	printf("%d %d\n", width, height);
	stsp->width = width;
	stsp->height = height;
	stsp->tiles_ptr = (tile_struct*) malloc(sizeof(tile_struct) * width * height);

	//read in tile data from file
	temp_buff = (char*) malloc(width * height);
	for(i = 0; i < height; i++) {
		if(-1 == fread(temp_buff + width * i, sizeof(char), width, file))
			err(-1, "Failed to fread in setup_game");
		if(fgetc(file) != '\n')
			errx(-1, "Trouble reading in setup_game");
	}
	fclose(file);

	//translate into tiles
	for(i = 0; i < width; i++) {
		for(j = 0; j < height; j++) {
			tile_struct temp_ts;
			int tile_index;

			tile_index = SHIP_TILES_INDEX(i, j, stsp);

			temp_ts.console_state_ptr = NULL;
			switch(temp_buff[tile_index]) {
				case ' ':
				case '-':
					temp_ts.type = TT_SPACE;
					break;
				case '#':
					temp_ts.type = TT_WALL;
					break;
				case 'X':
					temp_ts.type = TT_ALT_WALL;
					break;
				case '.':
					temp_ts.type = TT_FLOOR;
					break;
				case 'S':
					temp_ts.type = TT_SENSORS_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states + CI_SENSORS;
					break;
				case 'E':
					temp_ts.type = TT_ENGINES_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states + CI_ENGINES;
					break;
				case 'W':
					temp_ts.type = TT_WEAPONS_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states + CI_WEAPONS;
					break;
				case 'F':
					temp_ts.type = TT_FTL_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states + CI_FTL;
					break;
				case '@':
					temp_ts.type = TT_FLOOR;
					for(k = 0; k < MAX_PLAYERS; k++)
						if(gs->players[k].is_connected)
							{gs->players[k].x = i; gs->players[k].y = j;}
					break;
			}

			stsp->tiles_ptr[tile_index] = temp_ts;
		}
	}

	gs->curr_flow_state = FS_MAIN_GAME;
}
