#include "main_structs.h"

int is_console(enum _tile_type t) {
    return 
            t == TT_WEAPONS_CONSOLE ||
            t == TT_SENSORS_CONSOLE ||
            t == TT_ENGINES_CONSOLE ||
            t == TT_FTL_CONSOLE;
}

int is_walkable(enum _tile_type t) {
    return 
            t == TT_FLOOR ||
            t == TT_WEAPONS_CONSOLE ||
            t == TT_SENSORS_CONSOLE ||
            t == TT_ENGINES_CONSOLE ||
            t == TT_FTL_CONSOLE;
} 


static char passive_buff[] = "PASSIVE";
static char chasing_buff[] = "CHASING";
static char stable_buff[] = "STABLE";

char *flight_state_to_string(flight_state fs) {
	switch(fs) {
		case FS_PASSIVE:
			return passive_buff;
			break;
		case FS_CHASING:
			return chasing_buff;
			break;
		case FS_STABLE:
			return stable_buff;
			break;
		default:
			errx(-1, "unexpected flightstate in flight_state_to_string");
	}
}
