#ifndef UTIL_H
#define UTIL_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>

#define DEFAULT_PORT "9998"
#define LOCALHOST "127.0.0.1"
#define BACKLOG 5

#define MAX_PLAYERS 16
#define PLAYERS_TO_START 2

//frame delay in nanoseconds
#define FRAME_TIME (33 * 1000 * 1000)


//SCREEN WIDTH/HEIGHT TBD
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 30

#define SHIP_FILE "ship_layout.txt"

//Coordinates are inclusive/exclusive, like array indices
#define SHIP_PANEL_LEFT 0
#define SHIP_PANEL_RIGHT ((SCREEN_WIDTH)/2)
#define SHIP_PANEL_WIDTH ((SHIP_PANEL_RIGHT) - (SHIP_PANEL_LEFT))
#define SHIP_PANEL_TOP 0
#define SHIP_PANEL_BOTTOM SCREEN_HEIGHT-5
#define SHIP_PANEL_HEIGHT ((SHIP_PANEL_BOTTOM) - (SHIP_PANEL_TOP))

#define CONSOLE_PANEL_LEFT ((SHIP_PANEL_RIGHT) + 1)
#define CONSOLE_PANEL_RIGHT (SCREEN_WIDTH)
#define CONSOLE_PANEL_TOP 0
#define CONSOLE_PANEL_BOTTOM (SHIP_PANEL_BOTTOM)

#define ALERT_PANEL_LEFT 0
#define ALERT_PANEL_RIGHT (SCREEN_WIDTH)
#define ALERT_PANEL_TOP ((SHIP_PANEL_BOTTOM) + 1)
#define ALERT_PANEL_BOTTOM (SCREEN_HEIGHT)

//MAX CONSOLES TBD
#define MAX_CONSOLES 8
#define LOBBY_WIDTH 20
#define LOBBY_HEIGHT 20

//returns x if x is within the bounds
//otherwise returns the bound itself
int clamp(int x, int min, int max);

#endif
