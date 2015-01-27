#include "util.h"

int clamp(int x, int min, int max) {
	if(x > max)
		return max;
	if(x < min)
		return min;
	return x;
}

float fclamp(float x, float min, float max) {
	if(x > max)
		return max;
	if(x < min)
		return min;
	return x;
}

void render_strcpy(char* dest, char* src, int max_len) {
	int len = strlen(src);
	memcpy(dest, src, min(max_len, len));
}

int max(int a, int b) {return a > b ? a : b;}
int min(int a, int b) {return a < b ? a : b;}

int secs_to_frames(float s) {
	return (int)(s * 33.0f);
}

int rand_int(int min, int max) { //inclusive
	return (rand() % (max - min + 1)) + min;
}

float rand_float(float min, float max) {
	return min + (((float)rand() / (float)RAND_MAX) * (max - min));
}

float euclid_dist(float x, float y) {
	return sqrtf(x * x + y * y);
}
