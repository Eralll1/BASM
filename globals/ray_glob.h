#ifndef RAYGLOB
#define RAYGLOB
#include "../outer/raylib.h"

#define SCR_WIDTH 500
#define SCR_HEIGHT 500

#define PIXEL_SIZE 4

#define RAM_SIZE (SCR_WIDTH/PIXEL_SIZE) * (SCR_HEIGHT/PIXEL_SIZE)
static const long long __temp = RAM_SIZE;

#endif