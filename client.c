#include "client.h"

void init_client(char* ip, char* port){
  /*
    connects to stuff
    sets up curses
    goes to client_main_loop()
  */
  errx(-1, "init_client not implemented");
}

int connect_to(char* ip, char* port){
  //atomic, once it’s done, go to main loop
  errx(-1, "connect_to not implemented");
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
	//prepare this for george to send
	client_input_struct cis;

	//makes/sends client_input_state_struct;
	errx(-1, "send_input not implemented");
}

void get_render(){
	//Assume this will be prepared for rendering by george
	client_render_struct crs;

	//gets/draws client_render_struct;
	errx(-1, "get_render not implemented");
}
