#pragma once

#include <genesis.h>

#define TILE_SIZE 16

#define MAP_WIDTH 32 //48
#define MAP_WIDTH_PX (MAP_WIDTH * TILE_SIZE)

#define MAP_HEIGHT 32 // 16
#define MAP_HEIGHT_PX (MAP_HEIGHT * TILE_SIZE)

extern const Vect2D_s16 levelStartPos;

extern const u8 collisionMap[MAP_HEIGHT][MAP_WIDTH];
