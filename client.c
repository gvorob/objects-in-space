#include "client.h"
#include "main_structs.h"
#include <curses.h>

struct client_render_struct render;

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
