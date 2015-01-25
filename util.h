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
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 50

#define SHIP_FILE "ship_layout.txt"

#define SHIP_PANEL_LEFT 0
#define SHIP_PANEL_RIGHT SCREEN_HEIGHT/2 - 1
#define SHIP_PANEL_TOP 0
#define SHIP_PANEL_BOTTOM SCREEN_HEIGHT-5

#define CONSOLE_PANEL_LEFT SCREEN_WIDTH/2
#define CONSOLE_PANEL_RIGHT SCREEN_WIDTH 
#define CONSOLE_PANEL_TOP 0
#define CONSOLE_PANEL_BOTTOM SCREEN_HEIGHT-5

#define ALERT_PANEL_LEFT 0
#define ALERT_PANEL_RIGHT SCREEN_WIDTH
#define ALERT_PANEL_TOP SCREEN_HEIGHT - 4
#define ALERT_PANEL_BOTTOM SCREEN_HEIGHT

//MAX CONSOLES TBD
#define MAX_CONSOLES 8
#define LOBBY_WIDTH 30
#define LOBBY_HEIGHT 30

//returns x if x is within the bounds
//otherwise returns the bound itself
int clamp(int x, int min, int max);

#endif
