#include "client.h"

client_render_struct render;


void init_client(char* ip, char* port){
	/*
	  connects to stuff
	  sets up curses
	  goes to client_main_loop()
	  never returns
	*/

	
	printf("Starting client, connecting to %s on port %s\n", ip, port);
	int sd = connect_to(ip, port);
	printf("Connected\n");

	send(sd, "Hello World!\n", 13, 0);

	printf("Initializing curses\n");
	//DO THIS
		warnx("CURSES NOT YET IMPLEMENTED");
	printf("Done, going to main loop\n");
	
	client_main_loop(sd);
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
  errx(-1, "client_main_loop not implemented");
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

  //NEEDS TO SEND THE INPUT

  //makes/sends client_input_state_struct;
  //errx(-1, "send_input not implemented");
}

void get_render(){
  //gets/draws client_render_struct;
  //assuming render has got the correct array
  client_render_struct crs;
  
  clear(); 		//clears the screen
  move(0, 0);		//moves to start
  int i = 0;
  while (i > SCREEN_WIDTH * SCREEN_HEIGHT){
    addch(crs.render_data[i]);
  }
  //  errx(-1, "get_render not implemented");
}



