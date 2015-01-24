#include "client.h"

client_render_struct render;
int socket_d;


void init_client(char* ip, char* port){
	/*
	  connects to stuff
	  sets up curses
	  goes to client_main_loop()
	  never returns
	*/

	printf("Starting client, connecting to %s on port %s\n", ip, port);
	socket_d = connect_to(ip, port);
	printf("Connected at sd %d\n", socket_d);

	char temp_msg[HANDSHAKE_SIZE];
	strncpy(temp_msg, "Hello, I'm trying to connect to you!\n", HANDSHAKE_SIZE);
	send(socket_d, temp_msg, sizeof(temp_msg), 0);


	printf("Initializing curses\n");
	setup_ncurses();
	printf("Done, going to main loop\n");
	
	client_main_loop(socket_d);
}


int connect_to(char* ip, char* port){
  //atomic, once it’s done, go to main loop
        struct addrinfo hints, *servinfo;
        hints.ai_family = PF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        memset(&hints, 0, sizeof(hints));


        // Open connection to server here
        int rv = getaddrinfo(ip, port, &hints, &servinfo);
        if (rv != 0) {
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        }

        // loop through all the results and connect to the first we can
        struct addrinfo *p;
        int sockfd;
        for(p = servinfo; p != NULL; p = p->ai_next) {
                sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
                if (sockfd == -1) {
                        perror("client: socket");
                        continue;
                }

                if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
                        close(sockfd);
                        perror("client: connect");
                        continue;
                }

                break;
        }

        if (p == NULL) {
                err(-1, "client: failed to connect\n");
        }

        char s[INET6_ADDRSTRLEN];
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
                        s, sizeof s);
        printf("client: connecting to %s\n", s);

        freeaddrinfo(servinfo); // all done with this structure
	
	return sockfd;
}

void setup_ncurses(){
  initscr();		//initializes screen for ncurses
  noecho();		//keypresses don't show up
  curs_set(FALSE);	//don't show cursor
  cbreak();		//get key one at a time
  nodelay(stdscr, TRUE);	//makes it so getch doesn't wait for an input, instead gives ERR
  //errx(-1, "setup_ncurses not implemented");
}

void client_main_loop(int socket_d){
  //calls send_input(), get_render();
	while(1) {
		get_render();
		send_input();
	}
}

void send_input(){
	//this only does it for the current character being pressed
	//also, ncurses can only do 1 key at a time
	client_input_struct cis;


	char ch = getch();
	if (ch == UP){
	  cis.up = 1;
	}
	if (ch == DOWN){
	  cis.down = 1;
	}
	if (ch == LEFT){
	  cis.left = 1;
	}
	if (ch == RIGHT){
	  cis.right = 1;
	}
	if (ch == CONSOLE_LOCK){
	  cis.console_lock = 1;
	}

	
	if(-1 == send(socket_d, &cis, sizeof(cis), 0)) {
		if(errno == EAGAIN || errno == EWOULDBLOCK)
			warnx("nothing to recv");
		else
			warn("failed to recv");
	} else {

	}
}

void get_render(){
	//gets/draws client_render_struct;
	//assuming render has got the correct array
	client_render_struct crs;

	warnx("trying to recv");
	if(-1 == recv(socket_d, &crs, sizeof(crs), 0)) {
		warnx("frecvd");
		clear(); 		//clears the screen
		move(0, 0);		//moves to start
		if(errno == EAGAIN || errno == EWOULDBLOCK)
			warnx("nothing to recv");
		else
			warn("failed to recv");
		refresh();
	} else {

		int i;
		for(i = 0; i < sizeof(crs); i++) {
			if(!(i % SCREEN_WIDTH))
				fprintf(stderr,"\n");
			fprintf(stderr,"%c", ((char *)&crs)[i]);
		}
		warnx("recvd");
		clear(); 		//clears the screen
		move(0, 0);		//moves to start
		i = 0;
		while (i < SCREEN_HEIGHT){
			addnstr(crs.render_data + i * SCREEN_WIDTH, SCREEN_WIDTH);
			i++;
		}

		warnx("!!!%.10s!!!", crs.render_data);

		refresh();
	}
		//  errx(-1, "get_render not implemented");
}



