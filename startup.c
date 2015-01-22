#include "startup.h"

void init_server(gamestate_struct* gs){
  /*maybe init curses shenanigans
    setup_listening_socket(???)
    setup_connections_lobby(&gs)
    return into main loop
  */
  errx(-1, "init_server not implemented");
}

//Starts listening on port, returns socket
int setup_listening_socket(char* port){
	/*
	  (TBD) 
	  sets up socket for listening
	*/
	errx(-1, "setup_listening_socket not implemented");

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
  errx(-1, "setup_connections_lobby not implemented");
}

void update_input_connecting(client_struct* c, gamestate_struct* gs)
{
  /*
    gets input from the users that are already connected
   */
  errx(-1, "update_input_connecting not implemented");
}

void update_connecting(gamestate_struct* gs){
	/*  
	    let currently connected players move around
	    try to accept more
	    if all connections are done, call finalize_connections(...) and setup_game(...)
	    next loop around should then start playing properly
	*/

	/*TESTING CODE, NOT FINISHED YET
	sin_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
	if (new_fd == -1) {
		if(errno == EAGAIN || errno == EWOULDBLOCK)//if no connections
			continue;
		perror("accept");
		continue;
	}

	fcntl(new_fd, F_SETFL, O_NONBLOCK);

	inet_ntop(their_addr.ss_family,
		get_in_addr((struct sockaddr *)&their_addr),
		s, sizeof s);
	printf("server: got connection from %s\n", s);

	sockets[num_socks] = new_fd;
	num_socks++;
	//END TESTING */
}

void render_connecting(client_struct* c, gamestate_struct* gs){
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
