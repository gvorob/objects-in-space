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
