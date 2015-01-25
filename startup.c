#include "startup.h"

int listening_sd;

void init_server(gamestate_struct* gs){
	/*
	  setup_listening_socket(???)
	  setup_connections_lobby(&gs)
	  return into main loop
	*/

	listening_sd = setup_listening_socket(DEFAULT_PORT);

	printf("Preparing gamestate_struct\n");
	setup_connections_lobby(gs);
	return;
}

//Starts listening on port, returns socket
int setup_listening_socket(char* port){
	/*
	  (TBD) 
	  sets up socket for listening
	*/
        printf("Server starting\n");

	int sockfd;  // listen on sock_fd
	struct addrinfo hints, *servinfo, *p;
	//struct sockaddr_storage their_addr; // connector's address information
	//socklen_t sin_size;
	//struct sigaction sa;
	int yes=1;
	//char s[INET6_ADDRSTRLEN];
	int rv;

	//BEGIN PREPARING SOCKETS
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) { //prepares servinfo with own ip on port PORT
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL) 
		err(-1, "Failed to bind");

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	//SOCKETS PREPARED

	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	printf("Socket prepared for listening\n");

        return sockfd;
}

/*
sets flowstate to FC_CONNECTING
Inits/mallocs gamestate and sub-structs
set all clients/players as unconnected
prepares simple ship lobby, 5x5
*/
void setup_connections_lobby(gamestate_struct* gs){
	int i, j;
	ship_tiles_struct* stsp;

	gs->curr_flow_state = FS_CONNECTING;

	stsp = &(gs->shipstate.tiles);
	stsp->width = LOBBY_WIDTH;
	stsp->height = LOBBY_HEIGHT;
	stsp->tiles_ptr = (tile_struct *) malloc(sizeof(tile_struct) * LOBBY_WIDTH * LOBBY_HEIGHT);

	//Prepare simple square room
	for(i = 0; i < LOBBY_WIDTH; i++) {
		for(j = 0; j < LOBBY_HEIGHT; j++) {
			tile_struct temp_ts;
			temp_ts.console_state_ptr = NULL;

			if(
					i == 0 || 
					i + 1 == LOBBY_WIDTH || 
					j == 0 ||
					j + 1 == LOBBY_HEIGHT) {
				temp_ts.type = TT_WALL;
			} else {
				temp_ts.type = TT_FLOOR;
			}

			stsp->tiles_ptr[SHIP_TILES_INDEX(i, j, stsp)] = temp_ts;
		}
	}
	
	//players = (player_struct)malloc(sizeof(player_struct) * MAX_PLAYERS);
	for (i = 0; i < MAX_PLAYERS; i++){
		gs->clients[i].socket_d = -1;//setting stuff to its standard value
		gs->players[i].is_connected = 0;
	}

}

/*
	handles input for player number client_index
	just moves them around the lobby for now
	
	gs.players[client_index] contains the coordinates of the player
	gs.clients[client_index] contains the input from the client
 */
void update_input_connecting(int client_index, gamestate_struct* gs) {
	client_input_struct* cisp;
	player_struct* psp;

	warnx("updating input for player %d", client_index);
	
	cisp = &(gs->clients[client_index].curr_input_state);
	psp = &(gs->players[client_index]);

	if(cisp->up) {
		psp->y--;
	}
	if(cisp->down) {
		psp->y++;
	}
	if(cisp->left) {
		psp->x--;
	}
	if(cisp->right) {
		psp->x++;
	}

	psp->x = clamp(psp->x, 0, LOBBY_WIDTH - 1); 
	psp->y = clamp(psp->y, 0, LOBBY_HEIGHT - 1); 
}

void update_connecting(gamestate_struct* gs){
	int i;
	/*  
	    let currently connected players move around
	    try to accept more
	    if all connections are done, call finalize_connections(...) and setup_game(...)
	    next loop around should then start playing properly
	*/



	//HANDLE NEW CONNECTIONS
	int new_fd;

	new_fd = accept(listening_sd, NULL, 0);
	if (new_fd == -1) {
		if(errno == EAGAIN || errno == EWOULDBLOCK)//if no connections
			return;
		warn("Failed to accept in update_connecting");
		return;
	}

	//ONLY GETS HERE IF SOMEONE CONNECTED

	char woop[HANDSHAKE_SIZE];
	printf("server: got connection\n");
	read(new_fd, woop, HANDSHAKE_SIZE);
	woop[HANDSHAKE_SIZE] = 0;
	printf("%s\n", woop);

	//SET CLIENT/SOCKET/PLAYER INFO IN GS
	for(i = 0; i < MAX_PLAYERS; i++) {
		if(gs->clients[i].socket_d == -1) {
			memset(&(gs->clients[i]), 0, sizeof(client_struct));
			memset(&(gs->players[i]), 0, sizeof(player_struct));
			gs->clients[i].socket_d = new_fd;
			gs->players[i].is_connected = 1;

			gs->players[i].x = 1; //TEMP
			gs->players[i].y = 1;

			i = MAX_PLAYERS + 1;
		}
	}

	if(i == MAX_PLAYERS) {
		//CODE SHOULDN'T GET HERE UNLESS TOO MANY PLAYERS REACHED
		warnx("Too many players tried to connect");
		close(new_fd);
	}

	int total_players = 0;
	for(i = 0; i < MAX_PLAYERS; i++)
		if(gs->players[i].is_connected)
			total_players++;

	if(total_players == 2) {
		setup_game(gs);
	}
}

void render_connecting(int client_index, gamestate_struct* gs){
	char* rp; //render pointer
	ship_tiles_struct* stsp;
	player_struct ps;
	int i, j;

	rp = (gs->clients[client_index].render.render_data);
	stsp = &(gs->shipstate.tiles);

	//JUST TO DEMONSTRATE THAT IT WORKS
	static int temp = 0;
	temp++;
	char buff[25];
	sprintf(buff, "%d", temp);
	strcpy(rp, buff);
	//REPLACE THE ABOVE WITH PROPER RENDER CODE


	for(i = 0; i < LOBBY_WIDTH; i++) {
		for(j = 0; j < LOBBY_HEIGHT; j++) {
			tile_struct temp_ts;
			temp_ts = stsp->tiles_ptr[SHIP_TILES_INDEX(i, j, stsp)];

			if(temp_ts.type == TT_WALL) {
				rp[SCREEN_INDEX(i,j)] = '#';
			} else if(temp_ts.type == TT_FLOOR){
				rp[SCREEN_INDEX(i,j)] = '.';
			} else {
				rp[SCREEN_INDEX(i,j)] = '?';
			}
		}
	}

	for(i = 0; i < MAX_PLAYERS; i++) {
		ps = gs->players[i];
		if(ps.is_connected) {
			rp[SCREEN_INDEX(ps.x, ps.y)] = '@';
		}
	}
}

void finalize_connections(gamestate_struct* gs){
  /*
    close sockets etc etc
  */
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
					temp_ts.type = TT_FLOOR;
					break;
				case '#':
					temp_ts.type = TT_WALL;
					break;
				case 'X':
					temp_ts.type = TT_WALL;//TEMP
					break;
				case '.':
					temp_ts.type = TT_FLOOR;
					break;
				case 'S':
				case 'E':
				case 'W':
				case 'F':
					temp_ts.type = TT_FLOOR;//TEMP
					warnx("consoles not implemented yet");
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

	//gs->curr_flow_state = FS_MAIN_GAME;
}
