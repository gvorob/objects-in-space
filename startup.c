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

void setup_connections_lobby(gamestate_struct* gs){

	gs->curr_flow_state = FS_CONNECTING;
	/*
	  gs.shipstate.tiles //need to initialize and malloc stuff here
	*/
	player_struct players[MAX_PLAYERS];
	
	int i;
	//	for (i = 0; i < MAX_PLAYERS; i++){
		//	players[i] 
  
  /*
    prepare lobby, prepare clients
    lobby will be smaller MVP, maybe just 5x5 ship, very simple
    curr_flow_state = FS_CONNECTING
    prepares clients list
    +-------+
    |       |
    |       |
    |       |
    |       |
    |       |
    +-------+
  */
   
  
	warnx("setup_connections_lobby, not yet fully implemented");
}

void update_input_connecting(int client_index, gamestate_struct* gs)
{
	/*
	  gs.players[client_index] contains the coordinates of the player
	  gs.clients[client_index] contains the input from the client
	 */
	warnx("update_input_connecting not implemented");
}

void update_connecting(gamestate_struct* gs){
	/*  
	    let currently connected players move around
	    try to accept more
	    if all connections are done, call finalize_connections(...) and setup_game(...)
	    next loop around should then start playing properly
	*/

	int new_fd = accept(listening_sd, NULL, 0);
	if (new_fd == -1) {
		if(errno == EAGAIN || errno == EWOULDBLOCK)//if no connections
			return;
		warn("Failed to accept in update_connecting");
		return;
	}

	//fcntl(new_fd, F_SETFL, O_NONBLOCK);

	char woop[123];
	printf("server: got connection\n");
	read(new_fd, woop, 10);
	woop[10] = 0;
	printf("%s\n", woop);

}

void render_connecting(int client_index, gamestate_struct* gs){

}

void finalize_connections(gamestate_struct* gs){
  /*
    close sockets etc etc
  */
}

void setup_game(gamestate_struct* gs){
  /*
    prepare gamestate, load in ships, encounter, etc
  */
}
