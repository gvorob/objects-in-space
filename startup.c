#include "startup.h"

void init_server(gamestate_struct* gs){
  /*maybe init curses shenanigans
    setup_listening_socket(???)
    setup_connections_lobby(&gs)
    return into main loop
  */
  errx(-1, "init_server not implemented");
}

//int setup_listening_socket(???){
/*
  (TBD) 
  sets up socket for listening
  errx(-1, "setup_listening_socket not implemented");
*/
//}

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
