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
		printf("starting main loop\n");

		//getinput
		for(i = 0; i < MAX_PLAYERS; i++) {
				//checks if client is connected in function
				get_input(&(gs.clients[i]));
		}

		//update_input, update, and render
		for(i = 0; i < MAX_PLAYERS; i++)
				//checks if client is connected in function
				update_input(i, &gs);
		update(&gs);
		for(i = 0; i < MAX_PLAYERS; i++) {
				//checks if client is connected in function
				render(i, &gs);
		}


		//Sleep for 33 ms (~30fps)
		struct timespec temp_time = { 0, FRAME_TIME};
		nanosleep(&temp_time, NULL);
	}
}

void print_usage() {
	printf("Usage: ./space --client -ipxxx.xxx.xxx.xxx\n");
	printf("       ./space --server\n");

	exit(-1);
}

//modifies c->curr_input_state with latest input received from client
void get_input(client_struct* c) {
	if(c->socket_d == -1)
		return;

	if(-1 == recv(c->socket_d, &(c->curr_input_state), sizeof(client_input_struct), 0))
		warn("Failed to recv in get_input");
}

//delegates to other files
void update_input(int client_index, gamestate_struct* gs) {
	if(!gs->players[client_index].is_connected)
		return;

	switch(gs->curr_flow_state) {
		case FS_CONNECTING:
			update_input_connecting(client_index, gs);
			break;
		case FS_MAIN_GAME:
			update_input_main_game(client_index, gs);
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
			break;
		case FS_MAIN_GAME:
			update_main_game(gs);
			break;
		default:
			err(-1, "UNIMPLEMENTED FLOWSTATE");
			break;
	}
}

//delegates to other files
void render(int client_index, gamestate_struct* gs) {
	//Only send if a client is connected
	if(!gs->players[client_index].is_connected)
		return;

	switch(gs->curr_flow_state) {
		case FS_CONNECTING:
			render_connecting(client_index, gs);
			break;
		case FS_MAIN_GAME:
			render_main_game(client_index, gs);
			break;
		default:
			err(-1, "UNIMPLEMENTED FLOWSTATE");
			break;
	}

	if(-1 == send(gs->clients[client_index].socket_d, 
			&(gs->clients[client_index].render),
			sizeof(client_render_struct), 
			0))
		warn("failed to send in render");
	printf("sent render for player %d\n", client_index);
}

