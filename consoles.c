#include "consoles.h"

//WEAPONS =================
void init_weapons_console(
		gamestate_struct* gs) {
       
	weapons_console_state_struct* wcss;
	
	wcss = (weapons_console_state_struct *)malloc(sizeof(weapons_console_state_struct));
	wcss->current_weapon = 0;
	int i;
	for (i = 0; i < WEAPONS_MAX_WEAP;i++){
		wcss->target_xs[i] = 0;
		wcss->target_ys[i] = 0;
		
		if (i == 0){
			wcss->types[i] = WT_LASER;
		} else if (i == 1){
			wcss->types[i] = WT_MISSILE;
		} else {
			//well
		}
		wcss->weapon_charges[i] = 0;
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
		int metadata,
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	
	char *rp;
	int temp_x, temp_y;
	char title_string[] = "                   Weapons (WIP)";

	//prepare pointers
	rp = (gs->clients[client_index].render.render_data);

	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
			title_string, 
			CONSOLE_PANEL_WIDTH);
	if (metadata == 0){//aiming
		/*
		char aiming_string[] = "Aiming stuff here";
		
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP),
					  aiming_string,
					  CONSOLE_PANEL_WIDTH);
		*/
		//hard-coded thing right here
		
		//print current weapons
		char current_weapon_string[CONSOLE_PANEL_WIDTH];
		if(wcss->current_weapon == 0){
			strcpy(current_weapon_string,"Current Weapon: Lasers");
		} else {
			strcpy(current_weapon_string,"Current Weapon: Missiles");
		}
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP+2),
					  current_weapon_string,
					  CONSOLE_PANEL_WIDTH);


		//Print current target
		char current_weapon_target[CONSOLE_PANEL_WIDTH];
		sprintf(current_weapon_target,"Targeting: (%f,%f)",
				wcss->target_xs[wcss->current_weapon],
				wcss->target_ys[wcss->current_weapon]);
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP+4),
					  current_weapon_target,
					  CONSOLE_PANEL_WIDTH);		
		//print instructions
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT + 2, CONSOLE_PANEL_BOTTOM - 2),
			WEAPONS_AIMING_INSTRUCTIONS_STRING, 
			CONSOLE_PANEL_WIDTH - 2);

		//print instructions
		temp_x = CONSOLE_PANEL_LEFT + WEAPONS_CROSSHAIR_LEFT;
		temp_y = CONSOLE_PANEL_TOP + WEAPONS_CROSSHAIR_TOP;
		int marker;
		int origin_x = temp_x + WEAPONS_AIMING_BOUNDS * WEAPONS_CROSSHAIR_SCALE;
		int origin_y = temp_y + WEAPONS_AIMING_BOUNDS * WEAPONS_CROSSHAIR_SCALE;
		for(marker = -5; marker <= 5; marker++) {
			char m = (marker > 0 ? marker : -1 * marker) + '0';
			rp[SCREEN_INDEX(origin_x + marker * WEAPONS_CROSSHAIR_SCALE, origin_y)] = m;
			rp[SCREEN_INDEX(origin_x, origin_y + marker * WEAPONS_CROSSHAIR_SCALE)] = m;
		}

		origin_x += WEAPONS_CROSSHAIR_SCALE * wcss->target_xs[wcss->current_weapon];
		origin_y -= WEAPONS_CROSSHAIR_SCALE * wcss->target_ys[wcss->current_weapon];
		rp[SCREEN_INDEX(origin_x, origin_y)] = '+';
		
		
	} else if (metadata == 1){//status
		/*
		char status_string[] = " Test status here";

		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP),
					  status_string,
					  CONSOLE_PANEL_WIDTH);
		*/
		char current_weapon_string[CONSOLE_PANEL_WIDTH];
		char current_weapon_charge[CONSOLE_PANEL_WIDTH];
		if(wcss->current_weapon == 0){
			strcpy(current_weapon_string,"Lasers");
			sprintf(current_weapon_charge,"%lf/%d",wcss->weapon_charges[wcss->current_weapon],WT_LASER_CT);
		} else {
			strcpy(current_weapon_string,"Missiles");
			sprintf(current_weapon_charge,"%lf/%d",wcss->weapon_charges[wcss->current_weapon],WT_MISSILE_CT);
		}
		
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP+2),
					  current_weapon_string,
					  CONSOLE_PANEL_WIDTH);

		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP+4),
					  current_weapon_charge,
					  CONSOLE_PANEL_WIDTH);
		
		render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT + 2, CONSOLE_PANEL_BOTTOM - 2),
			WEAPONS_STATUS_INSTRUCTIONS_STRING, 
			CONSOLE_PANEL_WIDTH - 2);
		
	}
	
	
}
/*
takes input from the user, say wasd, to change the target of the weapon chosen
also can change weapon with another key
*/
void update_input_weapons_console(
		int client_index, 
		int metadata,
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {

	client_input_struct * cisp;
	cisp = &(gs->clients[client_index].curr_input_state);
	if (metadata == 0){//aiming
		//not drifting, just moving
		if (cisp->up){
			wcss->target_ys[wcss->current_weapon]+= 0.5;
		}
		if (cisp->down){
			wcss->target_ys[wcss->current_weapon]-= 0.5;
		}
		if (cisp->left){
			wcss->target_xs[wcss->current_weapon]-= 0.5;
		}
		if (cisp->right){
			wcss->target_xs[wcss->current_weapon]+= 0.5;
		}
		if (cisp->cancel){
			wcss->current_weapon = 1 - wcss->current_weapon;//0 and 1
		}
		wcss->target_xs[wcss->current_weapon] = 
				fclamp(wcss->target_xs[wcss->current_weapon],
						-1 * WEAPONS_AIMING_BOUNDS,
						WEAPONS_AIMING_BOUNDS);
		wcss->target_ys[wcss->current_weapon] = 
				fclamp(wcss->target_ys[wcss->current_weapon],
						-1 * WEAPONS_AIMING_BOUNDS,
						WEAPONS_AIMING_BOUNDS);
		
		//what we could do is text input
		//or we could use wasd and then e to confirm the spot to fire at
	} else if (metadata == 1){//status
		//nothing to do unless multiple tabs for further confusion
		if (cisp->cancel){
			wcss->current_weapon = 1 - wcss->current_weapon;//0 and 1
		}
		if (cisp->confirm){
			//fire
			//so much hardcoding
			float targeting_x;
			float targeting_y;
			targeting_x = wcss->target_xs[wcss->current_weapon];
			targeting_y = wcss->target_ys[wcss->current_weapon];		
			int damage;

			float enemy_x;
			float enemy_y;
			enemy_x = gs->encounter.enemy_location_x;
			enemy_y = gs->encounter.enemy_location_y;
			
			float offset;
			offset = cartesian_dist(targeting_x,targeting_y,enemy_x,enemy_y);
			if (wcss->current_weapon == 0){//laser
				if (wcss->weapon_charges[wcss->current_weapon] == WT_LASER_CT){
					//firing code
					damage = rand_int(WT_LASER_MINDMG,WT_LASER_MAXDMG);
					if (offset < 0.35){
						damage = (int)(0.9 * damage);
					} else if (offset < 0.70){
						damage = (int)(0.5 * damage);
					} else if (offset < 1.05){
						damage = (int)(0.2 * damage);
					} else {
						damage = 0;
					}
					//reset
					wcss->weapon_charges[wcss->current_weapon] = 0;
					gs->encounter.enemy_health-=damage;
				} else {
					//no fire
				}
			} else if (wcss->current_weapon == 1){//missile
				if (wcss->weapon_charges[wcss->current_weapon] == WT_MISSILE_CT){
					//firing code
					damage = rand_int(WT_MISSILE_MINDMG,WT_MISSILE_MAXDMG);
					if (offset < 0.35){
						//full damage
					} else if (offset < 0.70){
						//full damage
					} else if (offset < 1.05){
						damage = 0;
					} else {
						damage = 0;
					}
					//reset
					wcss->weapon_charges[wcss->current_weapon] = 0;
					gs->encounter.enemy_health-=damage;
				} else {
					//no fire
				}
			}
		}
	}
	
	//warnx("update_input_weapons_console not yet implemented");
}
/*
updates the console by making the charge bar update per weapon, as well as process
damage on enemy ship and a whole load of other combat mechanics
 */
void update_weapons_console(
		weapons_console_state_struct* wcss,
		gamestate_struct* gs) {
	//temporary until a better place is found for it
	int i;
	for (i = 0; i < WEAPONS_MAX_WEAP;i++){
		if ((i == 0 && wcss->weapon_charges[i] < WT_LASER_CT)
			|| (i == 1 && wcss->weapon_charges[i] < WT_MISSILE_CT)){
			wcss->weapon_charges[i]++;
		}
	}

	//warnx("update_weapons_console not yet implemented");
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
		int metadata,
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
			INSTRUCTIONS_STRING, 
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
			if(gs->encounter.enemy_health > 0) {
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
			} else {
				sprintf(temp_buff, "No hostiles detected");
				temp_rp = rp + SCREEN_INDEX(menu_left, menu_top);
				render_strcpy(temp_rp, temp_buff, CONSOLE_PANEL_WIDTH - 3);
			}
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
		int metadata,
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
	

	//Set default values
	ecss->current = 0;
	ecss->evade_selected = 0;

	//Add to gamestate
	gs->shipstate.console_states[CI_ENGINES] = ecss;  
};

void render_engines_console(
		int client_index, 
		int metadata,
		engines_console_state_struct* ecss,
		gamestate_struct* gs) {
	char *rp, *temp_rp;
	char temp_buff[SCREEN_WIDTH];
	char title_string[] = "                   Engines";
	int i;
	int temp_x, temp_y;
	char overheat_string[] = "OVERHEATING";
	
	//prepare pointers
	rp = (gs->clients[client_index].render.render_data);
	
	//Render title
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT, CONSOLE_PANEL_TOP), 
		      title_string, 
		      CONSOLE_PANEL_WIDTH);

	//render energy bar
	temp_x = ENGINES_CHARGE_BAR_LEFT + CONSOLE_PANEL_LEFT;
	temp_y = ENGINES_CHARGE_BAR_TOP + CONSOLE_PANEL_TOP;
	temp_rp = (char *)(rp + SCREEN_INDEX(temp_x, temp_y));

	int energy_width = (int)((gs->shipstate.engine_heat) * (float)ENGINES_CHARGE_BAR_WIDTH + 0.5);
	for(i = 0; i < ENGINES_CHARGE_BAR_WIDTH; i++) {
		temp_rp[i] = (i > energy_width) ? '-' : '+';
	}

	render_strcpy(rp + SCREEN_INDEX(temp_x + 35, temp_y - 1), 
			"Engine Heat", 
			CONSOLE_PANEL_WIDTH);
  
	//overheat message
	float new = gs->shipstate.engine_heat + 0.3;
	if(new >= 1.0f){
	  render_strcpy(rp + SCREEN_INDEX(temp_x + 35, temp_y + 1), 
			overheat_string, 
			CONSOLE_PANEL_WIDTH);
	}


	temp_x = CONSOLE_PANEL_LEFT + ENGINES_LEFT_MARGIN;
	temp_y = CONSOLE_PANEL_TOP + ENGINES_DESTS_TOP;

	//Print flight mode message
	render_strcpy(rp + 
			SCREEN_INDEX(temp_x, temp_y),
			"Choose trajectory type:",
			CONSOLE_PANEL_WIDTH - ENGINES_LEFT_MARGIN);

	temp_x += 2;
	temp_y += 1;
	
	//print options
	for(i = 0; i < ENGINES_MAX_STATES; i++) {
	  render_strcpy(rp + 
			SCREEN_INDEX(temp_x, temp_y + i),
			flight_state_to_string(i),
			CONSOLE_PANEL_WIDTH - ENGINES_LEFT_MARGIN);
	}

	temp_x -= 1;
	//Show current selection
	if(!ecss->evade_selected) {
		rp[SCREEN_INDEX(temp_x, temp_y + ecss->current)] = '>';
	}

	//Print evade message
	temp_x = CONSOLE_PANEL_LEFT + ENGINES_EVADE_BOX_LEFT;
	temp_y = CONSOLE_PANEL_TOP + ENGINES_EVADE_BOX_TOP;
	render_strcpy(rp + 
			SCREEN_INDEX(temp_x, temp_y + 1),
			"EVASIVE ACTION",
			CONSOLE_PANEL_WIDTH - ENGINES_LEFT_MARGIN);
	
	//Print evade box
	if(ecss->evade_selected) {
		temp_rp = rp + SCREEN_INDEX(temp_x, temp_y);
		for(i = strlen("EVASIVE ACTION") - 1; i >= 0; i--) {
			temp_rp[i] = '#';
			temp_rp[i + 2 * SCREEN_WIDTH] = '#';
		}
	}

	//print current flight state
	temp_x = CONSOLE_PANEL_LEFT + ENGINES_LEFT_MARGIN;
	temp_y = CONSOLE_PANEL_TOP + ENGINES_CURR_FLIGHT_STATE_TOP;
	strcpy(temp_buff, "Current flight mode: ");
	strcat(temp_buff, flight_state_to_string(gs->shipstate.curr_flight_state));
	render_strcpy(rp + 
			SCREEN_INDEX(temp_x, temp_y),
			temp_buff,
			CONSOLE_PANEL_WIDTH - ENGINES_LEFT_MARGIN);


	//=============================
	//Give info on incoming shots
	shot_struct *ssp, *first_shot;
	first_shot = NULL;
	int shortest_time = -1;
	
	//Find first shot that will hit
	for(ssp = &(gs->encounter.shots_list); ssp->next != NULL; ssp = ssp->next) {
		if(ssp->next->time_to_fly < shortest_time || shortest_time == -1) {
			shortest_time = ssp->next->time_to_fly;
			first_shot = ssp;
		}
	}

	//If there are any shots flying atm
	if(shortest_time != -1 && first_shot->next->type == ST_ENEMY) {
		//print that
		temp_x = CONSOLE_PANEL_LEFT + ENGINES_LEFT_MARGIN;
		temp_y = CONSOLE_PANEL_TOP + ENGINES_SHOT_INFO_TOP;
		sprintf(temp_buff, "Incoming fire in %.1fs", (float)shortest_time / 30);
		render_strcpy(rp +
			SCREEN_INDEX(temp_x, temp_y),
			temp_buff,
			CONSOLE_PANEL_WIDTH - ENGINES_LEFT_MARGIN);
	}
	

	//Print if I'm currently evading
	if(gs->shipstate.evasive_action > 0) {
		temp_x = CONSOLE_PANEL_LEFT + ENGINES_LEFT_MARGIN;
		temp_y = CONSOLE_PANEL_TOP + ENGINES_EVADING_STATUS_TOP;
		render_strcpy(rp + SCREEN_INDEX(temp_x, temp_y),
				"Performing evasive maneuvers!",
				CONSOLE_PANEL_WIDTH - 2);
	}
	
	//render instructions on bottom
	render_strcpy(rp + SCREEN_INDEX(CONSOLE_PANEL_LEFT + 2, CONSOLE_PANEL_BOTTOM - 1),
			INSTRUCTIONS_STRING, 
			CONSOLE_PANEL_WIDTH - 2);
	
}

void update_input_engines_console(
				  int client_index, 
				  int metadata,
				  engines_console_state_struct* ecss,
				  gamestate_struct* gs) {
	client_input_struct* cisp;
	cisp = &(gs->clients[client_index].curr_input_state);
  

	if(ecss->evade_selected) {
		if(cisp->left) {
			ecss->evade_selected = 0;
		}
	} else {
		if(cisp->up) {
			ecss->current--;
		}
		if(cisp->down) {
			ecss->current++;
		}
		if(cisp->right) {
			ecss->evade_selected = 1;
		}
	}

	ecss->current = clamp(ecss->current, 0, ENGINES_MAX_STATES - 1);

	if(cisp->confirm) {
		if (!ecss->evade_selected) { //If selecting a new flight mode
			gs->shipstate.curr_flight_state = ecss->current;
		} else { //If selectin evasive action

			float new = gs->shipstate.engine_heat + ENGINES_EVADE_HEAT_COST;
			if (new <= 1.0f){
				gs->shipstate.evasive_action = 1;
				gs->shipstate.engine_heat = gs->shipstate.engine_heat + ENGINES_EVADE_HEAT_COST;
			}
		}
	}

}
 void update_engines_console(
			     engines_console_state_struct* ecss,
			     gamestate_struct* gs) {
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
		int metadata,
		repairs_console_state_struct* wcss,
		gamestate_struct* gs) {
	warnx("render_repairs_console not yet implemented");
}

void update_input_repairs_console(
		int client_index, 
		int metadata,
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

	//Add into gamestate
	gs->shipstate.console_states[CI_FTL] = fcss;
};

void render_ftl_console(
		int client_index, 
		int metadata,
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
	int charge_width = (int)((gs->shipstate.ftl_charge) * (float)FTL_CHARGE_BAR_WIDTH + 0.5);
	temp_rp = (char *)(rp + SCREEN_INDEX(
			FTL_CHARGE_BAR_LEFT + CONSOLE_PANEL_LEFT,
			FTL_CHARGE_BAR_TOP + CONSOLE_PANEL_TOP));
	for(i = 0; i < FTL_CHARGE_BAR_WIDTH; i++) {
		temp_rp[i] = (i > charge_width) ? '-' : '+';
	}
	
	//CHARGED message
	if(gs->shipstate.ftl_charge >= 1.0f)
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
		int metadata,
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
}
//==========================
