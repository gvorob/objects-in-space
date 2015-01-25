#ifndef UTIL_H
#define UTIL_H

#include <err.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#define DEFAULT_PORT "9998"
#define LOCALHOST "127.0.0.1"
#define BACKLOG 5

#define MAX_PLAYERS 16

//frame delay in nanoseconds
#define FRAME_TIME (33 * 1000 * 1000)


//SCREEN WIDTH/HEIGHT TBD
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 50

//returns x if x is within the bounds
//otherwise returns the bound itself
int clamp(int x, int min, int max);

#endif
