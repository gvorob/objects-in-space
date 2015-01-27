#include "startup.h"

int listening_sd;

static char lobbyinstructions[30][100] = {
	"This is the lobby. When enough players connect, the main game will start.",
	"",
	"You are the '@'. You can walk around with w, a, s, and d.",
	"The '.'s are empty floor tiles, the '#' and '=' are wall tiles.",
	"",
	"Letters denote consoles, which are vital to your success",
	"(Unfortunately, consoles don't function in the lobby)",
	"",
	"Consoles are how you interact with the ship's systems. If you stand on a console",
	"and press space, you 'lock' into it and can interact with it.",
	"In a console, you can use w/a/s/d and also e (enter) or q (quit/cancel) to",
	"navigate its UI.",
	"",
	"To exit a console and resume walking around, press space again.",
	"",
	"In the game proper you will be part of a spaceship crew",
	"You will need to manage weapons through the weapons console and pilot the ship using",
	"the engines console. You'll also need a captain to man the sensors and help direct",
	"the others."
	};

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

	
	stsp->tiles_ptr[SHIP_TILES_INDEX(5,5,stsp)].type = TT_ALT_WALL;
	stsp->tiles_ptr[SHIP_TILES_INDEX(5,6,stsp)].type = TT_ALT_WALL;
	stsp->tiles_ptr[SHIP_TILES_INDEX(6,7,stsp)].type = TT_ALT_WALL;
	stsp->tiles_ptr[SHIP_TILES_INDEX(7,7,stsp)].type = TT_ALT_WALL;

	stsp->tiles_ptr[SHIP_TILES_INDEX(6,6,stsp)].type = TT_WEAPONS_CONSOLE;

	stsp->tiles_ptr[SHIP_TILES_INDEX(15,7,stsp)].type = TT_WALL;
	stsp->tiles_ptr[SHIP_TILES_INDEX(15,8,stsp)].type = TT_WALL;
	
	//players = (player_struct)malloc(sizeof(player_struct) * MAX_PLAYERS);
	for (i = 0; i < MAX_PLAYERS; i++){
		gs->clients[i].socket_d = -1;//setting stuff to its standard value
		gs->players[i].is_connected = 0;
		gs->players[i].is_at_console = 0;
	}

}

/*
	handles input for player number client_index
	just moves them around the lobby for now
	
	gs.players[client_index] contains the coordinates of the player
	gs.clients[client_index] contains the input from the client
 */
void update_input_connecting(int client_index, gamestate_struct* gs) {
	client_input_struct *cisp;
	player_struct *psp;
	ship_tiles_struct *stsp;

	warnx("updating input for player %d", client_index);
	
	cisp = &(gs->clients[client_index].curr_input_state);
	psp = &(gs->players[client_index]);
	stsp = &(gs->shipstate.tiles);

	int tempx = psp->x;
	int tempy = psp->y;

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

	tile_struct temp_tile = stsp->tiles_ptr[SHIP_TILES_INDEX(tempx, tempy, stsp)];

	if(is_walkable(temp_tile.type)) {
		psp->x = tempx;
		psp->y = tempy;
	}
}

/*  
	let currently connected players move around
	try to accept more
	if all connections are done, call finalize_connections(...) and setup_game(...)
	next loop around should then start playing properly
*/
void update_connecting(gamestate_struct* gs){
	int i;
	int new_fd;

	//HANDLE NEW CONNECTIONS

	new_fd = accept(listening_sd, NULL, 0);
	if (new_fd == -1) {
		if(errno == EAGAIN || errno == EWOULDBLOCK)//if no connections
			return;
		warn("Failed to accept in update_connecting");
		return;
	}

	//ONLY GETS HERE IF SOMEONE CONNECTED

	char handshake[HANDSHAKE_SIZE];
	printf("server: got connection\n");
	read(new_fd, handshake, HANDSHAKE_SIZE);
	handshake[HANDSHAKE_SIZE] = 0;
	printf("%s\n", handshake);

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

	//Count how many players have connected
	int total_players = 0;
	for(i = 0; i < MAX_PLAYERS; i++)
		if(gs->players[i].is_connected)
			total_players++;

	//All players connected, go to main game
	if(total_players == PLAYERS_TO_START) {
		finalize_connections(gs);
		setup_game(gs); //in main_game.c
	}
}

void render_connecting(int client_index, gamestate_struct* gs){
	char* rp; //render pointer
	ship_tiles_struct* stsp;
	player_struct ps;
	int i, j;
	int total_players = 0;

	rp = (gs->clients[client_index].render.render_data);
	stsp = &(gs->shipstate.tiles);


	for(i = 0; i < LOBBY_WIDTH; i++) {
		for(j = 0; j < LOBBY_HEIGHT; j++) {
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
					rp[SCREEN_INDEX(i,j)] = '=';
					break;
				default:
					rp[SCREEN_INDEX(i,j)] = '?';
					break;
			}
		}
	}

	for(i = 0; i < MAX_PLAYERS; i++) {
		ps = gs->players[i];
		if(ps.is_connected) {
			rp[SCREEN_INDEX(ps.x, ps.y)] = '@';
			total_players++;
		}
	}

	sprintf(
			rp + SCREEN_INDEX(LOBBY_WIDTH + 2, 0), 
			"%d/%d players connected", 
			total_players, 
			PLAYERS_TO_START);

	for(i = 0; i < 30; i++) {
	sprintf(
			rp + SCREEN_INDEX(LOBBY_WIDTH + 2, 2 + i),
			"%s", lobbyinstructions[i]);
	}
}

/*
	close sockets etc etc
*/
void finalize_connections(gamestate_struct* gs){
	close(listening_sd);
}
