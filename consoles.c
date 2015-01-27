#include "consoles.h"

//WEAPONS =================
void init_weapons_console(
		gamestate_struct* gs) {
       
	weapons_console_state_struct* wcss;
	
	wcss = (weapons_console_state_struct *)malloc(sizeof(weapons_console_state_struct));
	
	wcss->targx = 0;
	wcss->targy = 0;

	int i = 0;
	for (i = 0; i < WEAPONS_MAX_WEAP;i++){
		if (i == 0){
			snprintf(wcss->weapon_names[i],WEAPONS_MAX_WEAP_STRING,
					 "Missiles");
		} else if (i == 1){
			snprintf(wcss->weapon_names[i],WEAPONS_MAX_WEAP_STRING,
					 "Lasers");
		} else {
			snprintf(wcss->weapon_names[i],WEAPONS_MAX_WEAP_STRING,
					 "Weapon %d",i);
		}
		wcss->weapon_charges[i] = 100;
	}
	
	gs->shipstate.console_states[CI_WEAPONS] = wcss;
	
	//warnx("init_weapons_console not yet implemented");
};
/*renders the console overlay, which will have
the weapons systems displays as well as the
enemy ship layout
 */
void render_weapons_console(
		int client_index, 
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	
	warnx("render_weapons_console not yet implemented");

	char *rp;
	char title_string[] = "                   Weapons (WIP)";

	//prepare pointers
	rp = (gs->clients[client_index].render.render_data);

	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
			title_string, 
			CONSOLE_PANEL_WIDTH);
}
/*
takes input from the user, say wasd, to change the target of the weapon chosen
also can change weapon with another key
*/
void update_input_weapons_console(
		int client_index, 
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_input_weapons_console not yet implemented");
}
/*
updates the console by making the charge bar update per weapon, as well as process
damage on enemy ship and a whole load of other combat mechanics
 */
void update_weapons_console(
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_weapons_console not yet implemented");
}
//==========================

//SENSORS =================
void init_sensors_console(
		gamestate_struct* gs) {
	sensors_console_state_struct* scss;
	int i;

	//Malloc a struct
	scss = (sensors_console_state_struct*) malloc(sizeof(sensors_console_state_struct));
	if(scss == NULL)
		err(-1, "Failed to malloc in init_sensors_console");

	//Init members
	scss->current_menu = SMS_MAIN;
	scss->selected = 0;
	scss->enemy_coord_frame_counter = 0;
	for(i = 0; i < ENEMY_COORD_UPDATE_DELAY; i++) {
		scss->delayed_enemy_x_coords[i] = 0;
		scss->delayed_enemy_y_coords[i] = 0;
	}

	//Add into gamestate
	gs->shipstate.console_states[CI_SENSORS] = scss;
};

void render_sensors_console(
		int client_index, 
		sensors_console_state_struct* scss,
		gamestate_struct* gs) {

	char *rp, *temp_rp;
	//int temp_len;
	int i;
	char temp_buff[SCREEN_WIDTH];
	char title_string[] = "                   Sensors Panel";
	char main_menu_strings[3][SCREEN_WIDTH] = {
			"Ship info",
			"Enemy ship info",
			"Other info"
	};

	//Prepare pointers
	rp = (gs->clients[client_index].render.render_data);

	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
			title_string, 
			CONSOLE_PANEL_WIDTH);

	//render instructions on bottom
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT + 2, CONSOLE_PANEL_BOTTOM - 2),
			"Press e to confirm; q to go back; w/s move up/down; space to exit", 
			CONSOLE_PANEL_WIDTH - 2);
	
	int menu_top = CONSOLE_PANEL_TOP + 2;
	int menu_left = CONSOLE_PANEL_LEFT + 3;

	switch(scss->current_menu) {
		case SMS_MAIN:
			//print all options
			for(i = 0; i < SENSORS_MAIN_MENU_OPTIONS; i ++) {
				temp_rp = rp + SCREEN_INDEX(menu_left, menu_top + i * 2);
				render_strcpy(temp_rp, main_menu_strings[i], CONSOLE_PANEL_WIDTH - 3);
			}

			//print pointer
			rp[SCREEN_INDEX(menu_left - 1, menu_top + scss->selected * 2)] = '>';
			break;

		case SMS_SHIP:
			//print ship health
			sprintf(temp_buff, "Ship health: %d", gs->shipstate.health);
			temp_rp = rp + SCREEN_INDEX(menu_left, menu_top);
			render_strcpy(temp_rp, temp_buff, CONSOLE_PANEL_WIDTH - 3);

			//print flight state
			sprintf(temp_buff, "Currently flying in mode: ");
			strcat(temp_buff, flight_state_to_string(gs->shipstate.curr_flight_state));
			temp_rp = rp + SCREEN_INDEX(menu_left, menu_top + 2);
			render_strcpy(temp_rp, temp_buff, CONSOLE_PANEL_WIDTH - 3);
			break;
		case SMS_ENEMY:
			//print enemy health
			sprintf(temp_buff, "Enemy ship health: %d", gs->encounter.enemy_health);
			temp_rp = rp + SCREEN_INDEX(menu_left, menu_top);
			render_strcpy(temp_rp, temp_buff, CONSOLE_PANEL_WIDTH - 3);

			//prints enemy coords
			sprintf(temp_buff, "Enemy ship location: %.1f, %.1f",
					scss->delayed_enemy_x_coords[0],
					scss->delayed_enemy_y_coords[0]);
			temp_rp = rp + SCREEN_INDEX(menu_left, menu_top + 2);
			render_strcpy(temp_rp, temp_buff, CONSOLE_PANEL_WIDTH - 3);
			break;
		case SMS_OTHER:
			//print wip message
			sprintf(temp_buff, "This screen has not yet been implemented");
			temp_rp = rp + SCREEN_INDEX(menu_left, menu_top);
			render_strcpy(temp_rp, temp_buff, CONSOLE_PANEL_WIDTH - 3);
			break;
	}
}

void update_input_sensors_console(
		int client_index, 
		sensors_console_state_struct* scss,
		gamestate_struct* gs) {
	//used for turning select values into sms values
	static sensors_menu_state buttons[] = {SMS_SHIP, SMS_ENEMY, SMS_OTHER};
	client_input_struct* cisp;
	cisp = &(gs->clients[client_index].curr_input_state);


	if(scss->current_menu == SMS_MAIN) {
		if(cisp->confirm) {
			scss->current_menu = buttons[scss->selected];
		} else {
			if(cisp->up)
				scss->selected--;
			if(cisp->down)
				scss->selected++;
			scss->selected = clamp(scss->selected, 0, SENSORS_MAIN_MENU_OPTIONS - 1);
		}
	} else {
		if(cisp->cancel) {
			scss->current_menu = SMS_MAIN;
		}
	}
}

void update_sensors_console(
		sensors_console_state_struct* scss,
		gamestate_struct* gs) {
	//Handles delayed reporting of enemy position
	int i;

	//Take another datapoint once per second
	if(!scss->enemy_coord_frame_counter) {
		//shift all measurements down
		for(i = 0; i < ENEMY_COORD_UPDATE_DELAY - 2; i++) {
			scss->delayed_enemy_x_coords[i] = scss->delayed_enemy_x_coords[i + 1];
			scss->delayed_enemy_y_coords[i] = scss->delayed_enemy_y_coords[i + 1];
		}
		//take a new measurement
		scss->delayed_enemy_x_coords[i] = gs->encounter.enemy_location_x;
		scss->delayed_enemy_y_coords[i] = gs->encounter.enemy_location_y;

		//reset frame counter
		scss->enemy_coord_frame_counter = ENEMY_COORD_UPDATE_FRAMES;
	}

	scss->enemy_coord_frame_counter--;
}
//==========================



//ENGINES =================
void init_engines_console(
			  gamestate_struct* gs) {
  	engines_console_state_struct* ecss;
	
	//Mallocing
	ecss = (engines_console_state_struct*)malloc(sizeof(engines_console_state_struct));
	if(ecss == NULL)
		err(-1, "Failed to malloc in init_engines_console");
	
	//Init
	int i;
	for(i = 0; i < ENGINES_MAX_STATES-1; i++)
	  snprintf(ecss->states[i], 
		   FTL_MAX_DEST_STRING, 
		   "State: %s", 
		   flight_state_to_string(i));
	snprintf(ecss->states[i], FTL_MAX_DEST_STRING, "EVASIVE ACTION");
	ecss->engine_heat = 1;
	ecss->curr_flight_state = FS_PASSIVE;
	ecss->current = 0;
	ecss->evasive_action = 0;
	//Add to gamestate
	gs->shipstate.console_states[CI_ENGINES] = ecss;  
};

void render_engines_console(
		int client_index,
		engines_console_state_struct* ecss,
		gamestate_struct* gs) {
	char *rp, *temp_rp;
	char title_string[] = "                   Engines";
	int i;
	char overheat_string[] = "                    OVERHEATING";
	
	//prepare pointers
	rp = (gs->clients[client_index].render.render_data);
	
	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
		      title_string, 
		      CONSOLE_PANEL_WIDTH);

	//render energy bar
	int energy_width = (int)((ecss->engine_heat) * (float)ENGINES_CHARGE_BAR_WIDTH + 0.5);
  
	temp_rp = (char *)(rp + SCREEN_INDEX(
					     ENGINES_CHARGE_BAR_LEFT + CONSOLE_PANEL_LEFT,
					     ENGINES_CHARGE_BAR_TOP + CONSOLE_PANEL_TOP));
	for(i = 0; i < ENGINES_CHARGE_BAR_WIDTH; i++) {
		temp_rp[i] = (i > energy_width) ? '-' : '+';
  }
  
	//overheat message
	if(ecss->engine_heat >= 1.0f)
	  		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, ENGINES_CHARGE_BAR_TOP + 1), 
				overheat_string, 
				CONSOLE_PANEL_WIDTH);
	
	//Show states
	for(i = 0; i < ENGINES_MAX_STATES; i++) {
	  render_strcpy(rp + 
			SCREEN_INDEX(
				     CONSOLE_PANEL_LEFT + ENGINES_LEFT_MARGIN, 
				     CONSOLE_PANEL_TOP + ENGINES_DESTS_TOP + i * 2), 
			ecss->states[i], 
			CONSOLE_PANEL_WIDTH - ENGINES_LEFT_MARGIN);
	}
	
	//Show current state
	rp[SCREEN_INDEX(
			CONSOLE_PANEL_LEFT + ENGINES_LEFT_MARGIN - 1, 
			CONSOLE_PANEL_TOP + ENGINES_DESTS_TOP + ecss->current * 2)] = '>';
}

void update_input_engines_console(
				  int client_index, 
				  engines_console_state_struct* ecss,
				  gamestate_struct* gs) {
  client_input_struct* cisp;
  cisp = &(gs->clients[client_index].curr_input_state);
  
  if(cisp->up) {
    ecss->current--;
  }
  if(cisp->down) {
    ecss->current++;
  }
  
  ecss->current = clamp(ecss->current, 0, ENGINES_MAX_STATES - 1);
  //warnx("update_input_engines_console not yet implemented");
}

void update_engines_console(
			    engines_console_state_struct* ecss,
			    gamestate_struct* gs) {
  gs->shipstate.engine_heat = ecss->engine_heat;
  gs->shipstate.curr_flight_state = ecss->curr_flight_state;

  int time_to_cool = secs_to_frames(10);
  float cool_per_frame = (float)(1 / (float)time_to_cool);
  ecss->engine_heat = fclamp((ecss->engine_heat)-(cool_per_frame), 0, 1);

  int evasion_over = secs_to_frames(2);
  float down_per_frame = (float)(1 / (float)evasion_over);
  ecss->evasive_action = fclamp((ecss->evasive_action)-(down_per_frame), 0, 1);
  //warnx("update_engines_console not yet implemented");
}
//==========================



//REPAIRS =================
void init_repairs_console(
		gamestate_struct* gs) {
	warnx("init_repairs_console not yet implemented");
};

void render_repairs_console(
		int client_index, 
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("render_repairs_console not yet implemented");
}

void update_input_repairs_console(
		int client_index, 
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
  	warnx("update_input_repairs_console not yet implemented");
}

void update_repairs_console(
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("update_repairs_console not yet implemented");
}
//==========================



//FTL =================
void init_ftl_console(
		gamestate_struct* gs) {
	ftl_console_state_struct* fcss;
	int i;

	//Malloc a struct
	fcss = (ftl_console_state_struct*) malloc(sizeof(ftl_console_state_struct));
	if(fcss == NULL)
		err(-1, "Failed to malloc in init_ftl_console");

	//Init members
	for(i = 0; i < FTL_MAX_DESTS; i++)
		snprintf(fcss->destinations[i], FTL_MAX_DEST_STRING, "Destination #%d", i);
	fcss->current = 0;
	fcss->charge = 0;

	//Add into gamestate
	gs->shipstate.console_states[CI_FTL] = fcss;
};

void render_ftl_console(
		int client_index, 
		ftl_console_state_struct *fcss,
		gamestate_struct *gs) {

	char *rp, *temp_rp;
	//int left_offset;
	int i;
	char title_string[] = "                   FTL Drive";
	char charged_string[] = "                    CHARGED";

	//prepare pointers
	rp = (gs->clients[client_index].render.render_data);
  
	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
			title_string, 
			CONSOLE_PANEL_WIDTH);

	//render charge bar
	int charge_width = (int)((fcss->charge) * (float)FTL_CHARGE_BAR_WIDTH + 0.5);
	temp_rp = (char *)(rp + SCREEN_INDEX(
			FTL_CHARGE_BAR_LEFT + CONSOLE_PANEL_LEFT,
			FTL_CHARGE_BAR_TOP + CONSOLE_PANEL_TOP));
	for(i = 0; i < FTL_CHARGE_BAR_WIDTH; i++) {
		temp_rp[i] = (i > charge_width) ? '-' : '+';
	}
	
	//CHARGED message
	if(fcss->charge >= 1.0f)
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, FTL_CHARGE_BAR_TOP + 1), 
				charged_string, 
				CONSOLE_PANEL_WIDTH);

	//Show destinations
	for(i = 0; i < FTL_MAX_DESTS; i++) {
		render_strcpy(rp + 
				SCREEN_INDEX(
						CONSOLE_PANEL_LEFT + FTL_LEFT_MARGIN, 
						CONSOLE_PANEL_TOP + FTL_DESTS_TOP + i * 2), 
				fcss->destinations[i], 
				CONSOLE_PANEL_WIDTH - FTL_LEFT_MARGIN);
	}

	//Show current destination
	rp[SCREEN_INDEX(
				CONSOLE_PANEL_LEFT + FTL_LEFT_MARGIN - 1, 
				CONSOLE_PANEL_TOP + FTL_DESTS_TOP + fcss->current * 2)] = '>';
}

void update_input_ftl_console(
		int client_index, 
		ftl_console_state_struct* fcss,
		gamestate_struct* gs) {
	
	client_input_struct* cisp;
	cisp = &(gs->clients[client_index].curr_input_state);

	if(cisp->up) {
		fcss->current--;
	}
	if(cisp->down) {
		fcss->current++;
	}

	fcss->current = clamp(fcss->current, 0, FTL_MAX_DESTS - 1);
}

void update_ftl_console(
		ftl_console_state_struct* fcss,
		gamestate_struct* gs) {
	//Has to charge in 20 seconds
	//20 * 30 = 600 frams
	fcss->charge = fclamp(fcss->charge + 1.0f/600.0f, 0, 1);
}
//==========================
