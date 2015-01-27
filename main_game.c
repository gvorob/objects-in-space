#include "main_game.h"

void update_input_main_game(int client_index, gamestate_struct* gs) {
	client_input_struct* cisp;
	player_struct* psp;
	ship_tiles_struct* stsp;

	//Prepare temporary pointers
	cisp = &(gs->clients[client_index].curr_input_state);
	psp = &(gs->players[client_index]);
	stsp = &(gs->shipstate.tiles);

	warnx("updating input for player %d", client_index);

	//Handle locking to console
	if(cisp->console_lock) {
		//If standing on a console
		tile_struct temp_tile;
		temp_tile = stsp->tiles_ptr[SHIP_TILES_INDEX(psp->x, psp->y, stsp)];
		if(is_console(temp_tile.type)) {
			psp->is_at_console = !(psp->is_at_console);
		}
	}

	//If not at a console, walk around
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

		tempx = clamp(tempx, 0, stsp->width - 1); 
		tempy = clamp(tempy, 0, stsp->height - 1); 

		temp_tile = stsp->tiles_ptr[SHIP_TILES_INDEX(tempx, tempy, stsp)];

		if(is_walkable(temp_tile.type)) {
			psp->x = tempx;
			psp->y = tempy;
		}
	} else { //Otherwise, delegate to the appropriate console
		tile_struct temp;
		int metadata;
		void* csp; //console state pointer

		temp = stsp->tiles_ptr[SHIP_TILES_INDEX(psp->x, psp->y, stsp)];
		csp = temp.console_state_ptr;
		metadata = temp.metadata;
		switch(temp.type) {
			case TT_WEAPONS_CONSOLE:
				update_input_weapons_console(
						client_index, 
						metadata,
						(weapons_console_state_struct*) csp,
						gs);
				break;			
			case TT_SENSORS_CONSOLE:
				update_input_sensors_console(
						client_index, 
						metadata,
						(sensors_console_state_struct*) csp,
						gs);
				break;			
			case TT_ENGINES_CONSOLE:
				update_input_engines_console(
						client_index, 
						metadata,
						(engines_console_state_struct*) csp,
						gs);
				break;			
			case TT_FTL_CONSOLE:
				update_input_ftl_console(
						client_index, 
						metadata,
						(ftl_console_state_struct*) csp,
						gs);
				break;			
			case TT_REPAIRS_CONSOLE:
				update_input_repairs_console(
						client_index, 
						metadata,
						(repairs_console_state_struct*) csp,
						gs);
				break;			
			default:
				warnx("Is at console but isn't at console. ???");
				break;
		}
	}
}

void update_main_game(gamestate_struct* gs) {
	shot_struct *ssp;
	//update shots
		//walk the LL
	for(ssp = &(gs->encounter.shots_list); ssp->next != NULL; ) {
		ssp->next->time_to_fly--;

		//if hit
		if(!ssp->next->time_to_fly) {
			//deal damage
			gs->shipstate.health--;
			//delete the shot
			ssp->next = ssp->next->next;

		} else {
			//Walk the list manually so that we can delete things as we go
			ssp = ssp->next;
		}
	}

	//Update ftl charge
		//Has to charge in 20 seconds
		//20 * 30 = 600 frams
	gs->shipstate.ftl_charge = 
			fclamp(gs->shipstate.ftl_charge + 1.0f/secs_to_frames(SHIP_FTL_CHARGE_TIME), 0, 1);

	//update enemy
	encounter_move_enemy(gs);
	encounter_try_firing(gs);

	//delegate to consoles as well
	update_sensors_console(
			(sensors_console_state_struct *) gs->shipstate.console_states[CI_SENSORS],
			gs);
	update_weapons_console(
			(weapons_console_state_struct *) gs->shipstate.console_states[CI_WEAPONS],
			gs);
	update_engines_console(
			(engines_console_state_struct *) gs->shipstate.console_states[CI_ENGINES],
			gs);
	update_ftl_console(
			(ftl_console_state_struct *) gs->shipstate.console_states[CI_FTL],
			gs);
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

	//Draw crossbars
	for(j = 0; j < SHIP_PANEL_BOTTOM; j++) {
			rp[SCREEN_INDEX(SHIP_PANEL_RIGHT,j)] = '|';
	}
	for(i = 0; i < SCREEN_WIDTH; i++) {
			rp[SCREEN_INDEX(i, SHIP_PANEL_BOTTOM)] = '=';
	}

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
					rp[SCREEN_INDEX(i,j)] = 'W'; //temporary, just marks as console
					break;
				case TT_SENSORS_CONSOLE:
					rp[SCREEN_INDEX(i,j)] = 'S'; //temporary, just marks as console
					break;
				case TT_ENGINES_CONSOLE:
					rp[SCREEN_INDEX(i,j)] = 'E'; //temporary, just marks as console
					break;
				case TT_REPAIRS_CONSOLE:
					rp[SCREEN_INDEX(i,j)] = 'R'; //temporary, just marks as console
					break;
				case TT_FTL_CONSOLE:
					rp[SCREEN_INDEX(i,j)] = 'F'; //temporary, just marks as console
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

	//Draw in laser shots
	shot_struct *ssp;

	//walk the LL
	for(ssp = &(gs->encounter.shots_list); ssp->next != NULL; ssp = ssp->next) {
		int temp_x, temp_y;
		shot_struct *temp_ssp = ssp->next;

		//If shot is onscreen
		if(temp_ssp->time_to_fly < temp_ssp->entry_time) {
			//goes from 1 -> 0 as shot approaches target from edge of screen
			float rev_percent_of_trip = ((float)temp_ssp->time_to_fly / (float)temp_ssp->entry_time);

			temp_x = temp_ssp->target_x + 
					(int)(rev_percent_of_trip * (temp_ssp->entry_x - temp_ssp->target_x));
			temp_y = temp_ssp->target_y + 
					(int)(rev_percent_of_trip * (temp_ssp->entry_y - temp_ssp->target_y));

			if(temp_x != clamp(temp_x, 0, SCREEN_WIDTH))
				err(-1, "WOOPX %d %f", temp_x, rev_percent_of_trip);
			if(temp_y != clamp(temp_y, 0, SCREEN_HEIGHT))
				err(-1, "WOOPY %d %f", temp_y, rev_percent_of_trip);

			rp[SCREEN_INDEX(temp_x, temp_y)] = '+';
		}
	}

	//Delegate rendering to consoles (if applicable)
	ps = gs->players[client_index];
	if(ps.is_at_console) {
		tile_struct temp;
		int metadata;
		void* csp; //console state pointer

		temp = stsp->tiles_ptr[SHIP_TILES_INDEX(ps.x, ps.y, stsp)];
		csp = temp.console_state_ptr;
		metadata = temp.metadata;
		switch(temp.type) {
			case TT_WEAPONS_CONSOLE:
				render_weapons_console(
						client_index, 
						metadata,
						(weapons_console_state_struct*) csp,
						gs);
				break;			
			case TT_SENSORS_CONSOLE:
				render_sensors_console(
						client_index, 
						metadata,
						(sensors_console_state_struct*) csp,
						gs);
				break;			
			case TT_ENGINES_CONSOLE:
				render_engines_console(
						client_index, 
						metadata,
						(engines_console_state_struct*) csp,
						gs);
				break;			
			case TT_FTL_CONSOLE:
				render_ftl_console(
						client_index, 
						metadata,
						(ftl_console_state_struct*) csp,
						gs);
				break;			
			case TT_REPAIRS_CONSOLE:
				render_repairs_console(
						client_index, 
						metadata,
						(repairs_console_state_struct*) csp,
						gs);
				break;			
			default:
				warnx("Is at console but isn't at console. ???");
				break;
		}
	}

	//DEBUGGING
	char temp_buff[999];
	sprintf(temp_buff,"is_at_console:%d", ps.is_at_console);
	render_strcpy(rp + SCREEN_INDEX(ALERT_PANEL_LEFT, ALERT_PANEL_TOP), temp_buff, SCREEN_WIDTH);
	sprintf(temp_buff,"enemy_location: %f, %f", 
			gs->encounter.enemy_location_x,
			gs->encounter.enemy_location_y);
	render_strcpy(rp + SCREEN_INDEX(ALERT_PANEL_LEFT, ALERT_PANEL_TOP + 1), temp_buff, SCREEN_WIDTH);
	sprintf(temp_buff,"enemy target: %f, %f", 
			gs->encounter.target_x,
			gs->encounter.target_y);
	render_strcpy(rp + SCREEN_INDEX(ALERT_PANEL_LEFT, ALERT_PANEL_TOP + 2), temp_buff, SCREEN_WIDTH);
	sprintf(temp_buff,"enemy loiter_time: %d", 
			gs->encounter.loiter_time);
	render_strcpy(rp + SCREEN_INDEX(ALERT_PANEL_LEFT, ALERT_PANEL_TOP + 3), temp_buff, SCREEN_WIDTH);
	
	//Mark bottom right corner
	rp[SCREEN_INDEX(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1)] = '/';
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

	//initialize ship stats
	gs->shipstate.health = SHIP_MAX_HEALTH; 
	gs->shipstate.ftl_charge = 0;
	gs->shipstate.aim_x = 0;
	gs->shipstate.aim_y = 0;
	
	//prepare consoles
	for(i = 0; i < MAX_CONSOLES; i++)
		gs->shipstate.console_states[i] = NULL;
	init_weapons_console(gs);
	init_engines_console(gs);
	init_sensors_console(gs);
	init_ftl_console(gs);

	//========================
	//LOAD IN SHIP LAYOUT

	//free existing tile array
	free(stsp->tiles_ptr);
	stsp->tiles_ptr = NULL;


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
			temp_ts.metadata = 0;
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
					temp_ts.console_state_ptr = gs->shipstate.console_states[CI_SENSORS];
					break;
				case 'E':
					temp_ts.type = TT_ENGINES_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states[CI_ENGINES];
					break;
				case 'A':
					temp_ts.metadata = 1;
				case 'W':
					temp_ts.type = TT_WEAPONS_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states[CI_WEAPONS];
					break;
				case 'F':
					temp_ts.type = TT_FTL_CONSOLE;
					temp_ts.console_state_ptr = gs->shipstate.console_states[CI_FTL];
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

	//DONE LOADING IN SHIP LAYOUT
	//===============================

	//prepare enemy and other non-ship data
	setup_encounter(gs);

	//Set flow state
	gs->curr_flow_state = FS_MAIN_GAME;
}

