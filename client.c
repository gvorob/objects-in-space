#include "client.h"


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

void *get_in_addr(struct sockaddr *sa) {
	return sa->sa_family == AF_INET
		? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
		: (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
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

//setup_ncurses(???){
/*
  errx(-1, "setup_ncurses not implemented");
*/
//}

void client_main_loop(int socket_d){
  //calls send_input(), get_render();
  errx(-1, "client_main_loop not implemented");
}

void send_input(){
  //makes/sends client_input_state_struct;
  errx(-1, "send_input not implemented");
}

void get_render(){
  //gets/draws client_render_struct;
  errx(-1, "get_render not implemented");
}



