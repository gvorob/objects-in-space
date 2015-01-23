#include "controller.h"

int main(int argc, char *argv[]) {
	int is_client;
	char* ip_string;
	gamestate_struct gs;
	

	if(argc != 3 && argc != 2)
		print_usage();

	if(!strcmp(argv[1], "--client")) {
		is_client = 1;

		if(argc == 3 && !strncmp(argv[2], "-ip", 3))
			ip_string = argv[2] + 3;
		else
			print_usage();

		printf("Starting as client, ip=%s\n", ip_string);
	} else if(!strcmp(argv[1], "--server")) {
		is_client = 0;

		if(argc != 2)
			print_usage();

		printf("Starting as server\n");
	} else {
		print_usage();
	}


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

void print_usage() {
	printf("Usage: ./space --client -ipxxx.xxx.xxx.xxx\n");
	printf("       ./space --server\n");

	exit(-1);
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
