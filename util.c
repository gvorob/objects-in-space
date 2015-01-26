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
	memcpy(dest, src, max(max_len, len));
}

int max(int a, int b) {return a > b ? a : b;}
int min(int a, int b) {return a < b ? a : b;}
