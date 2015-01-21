#include "controller.h"

int main(int argc, char *argv[]) {
	
	int is_client = 1; //TEMP

	if(is_client) {
		init_client("127.0.0.1", 9999);//TEMP
		//Won't return from this
	}

	gamestate_struct gs;

	//server
	init_server(&gs);
	while(1) {
		int i;

		for(i = 0; i < MAX_PLAYERS; i++) {
			if(gs.clients[i].socket_d != -1)
				get_input(&(gs.clients[i]));
		}
		for(i = 0; i < MAX_PLAYERS; i++) {
			update_input(&(gs.clients[i]),&gs);
		}
		update(&gs);
		for(i = 0; i < MAX_PLAYERS; i++) {
			render(,gs);
		}
	}
}





void get_input(client_struct* c) {
	errx(-1, "get_input not implemented");
}

void update_input(client_struct* c, gamestate_struct* gs) {
	errx(-1, "update_input not implemented");
	update_input_connecting(c, gs);
}

void update(gamestate_struct* gs) {
	//delegates to other files
	errx(-1, "update not implemented");
	update_connecting(gs);
}

void render(client_struct* c, gamestate_struct* gs) {
	errx(-1, "render not implemented");
	render_connecting(c, gs);
}
