#include "controller.h"

int main(int argc, char *argv[]) {
	gamestate_struct gs;
	
	int is_client = 1; //TEMP
	warnx("Starting as client/server is hardcoded for now");

	if(is_client) {
		//Client main loop in here
		//Won't return from this
		init_client(LOCALHOST, DEFAULT_PORT);//TEMP
		err(0, "Client returned from main loop");
	}

	
	//server
	init_server(&gs);

	//MAIN LOOP
	while(1) {
		int i;

		//getinput
		for(i = 0; i < MAX_PLAYERS; i++) {
			if(gs.clients[i].socket_d != -1)
				get_input(&(gs.clients[i]));
		}

		//update_input, update, and render
		for(i = 0; i < MAX_PLAYERS; i++)
			update_input(&(gs.clients[i]), &gs);
		update(&gs);
		for(i = 0; i < MAX_PLAYERS; i++)
			render(&(gs.clients[i]), &gs);
	}
}




//modifies c->curr_input_state with latest input received from client
void get_input(client_struct* c) {
	errx(-1, "get_input not implemented");
}

//delegates to other files
void update_input(int client_index, gamestate_struct* gs) {
	switch(gs->curr_flow_state) {
		case FS_CONNECTING:
			update_input_connecting(c, gs);
			break;
		default:
			err(-1, "UNIMPLEMENTED FLOWSTATE");
			break;
	}
}

//delegates to other files
void update(gamestate_struct* gs) {
	//delegates to other files
	switch(gs->curr_flow_state) {
		case FS_CONNECTING:
			update_connecting(gs);
		default:
			err(-1, "UNIMPLEMENTED FLOWSTATE");
			break;
	}
}

//delegates to other files
void render(int client_index, gamestate_struct* gs) {
	switch(gs->curr_flow_state) {
		case FS_CONNECTING:
			render_connecting(c, gs);
		default:
			err(-1, "UNIMPLEMENTED FLOWSTATE");
			break;
	}
}
