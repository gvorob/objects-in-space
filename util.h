#ifndef UTIL_H
#define UTIL_H

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define DEFAULT_PORT "9998"
#define LOCALHOST "127.0.0.1"
#define BACKLOG 5

#define MAX_PLAYERS 16
#define PLAYERS_TO_START 2

//frame delay in nanoseconds
#define FRAME_TIME (33 * 1000 * 1000)
#define FPS 30


//SCREEN WIDTH/HEIGHT TBD
#define SCREEN_WIDTH 132
#define SCREEN_HEIGHT 43

#define SHIP_FILE "ship_layout.txt"

//Coordinates are inclusive/exclusive, like array indices
#define SHIP_PANEL_LEFT 0
#define SHIP_PANEL_RIGHT 45
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
#define CONSOLE_PANEL_WIDTH ((CONSOLE_PANEL_RIGHT) - (CONSOLE_PANEL_LEFT))
#define ALERT_PANEL_TOP ((SHIP_PANEL_BOTTOM) + 1)
#define ALERT_PANEL_BOTTOM (SCREEN_HEIGHT)
#define CONSOLE_PANEL_HEIGHT ((CONSOLE_PANEL_BOTTOM) - (CONSOLE_PANEL_TOP))

#define GAME_OVER_LEFT 6
#define GAME_OVER_TOP 17


//MAX CONSOLES TBD
#define MAX_CONSOLES 8
#define LOBBY_WIDTH 20
#define LOBBY_HEIGHT 20


//returns x if x is within the bounds
//otherwise returns the bound itself
int clamp(int x, int min, int max);
float fclamp(float x, float min, float max);

//copies the string without copying the terminating null
void render_strcpy(char* dest, char* src, int max_len);

//what it says on the tine
int max(int a, int b);
int min(int a, int b);

//Turns a float time in seconds into frames
int secs_to_frames(float s);

//rands
int rand_int(int min, int max); //inclusive
float rand_float(float min, float max);

//euclidean distance
float euclid_dist(float x, float y);

//cartesian distance
float cartesian_dist(float x1, float y1, float x2, float y2);
#endif
