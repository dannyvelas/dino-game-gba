#ifndef UTIL_H
#define UTIL_H

// our sprites are 4x4 tiles
#define SPRITE_TILE_AMT 16
// one tile is 8x8 pixels
#define TILE_PIXEL_DIM 8
// one tile is 8 32-bit ints
#define TILE_32BIT_AMT 8

// lets use 32x32 tiles for our background
#define BG_DIM 32
// lets make floor 4 screenblock entries off ground
#define FLOOR_SCR_ENTRY_Y (SCREEN_HEIGHT_T - 4)
// lets make our sprites be 21 pixels above the floor
#define SPRITE_FLOOR_PIXELS_Y ((FLOOR_SCR_ENTRY_Y * TILE_PIXEL_DIM) - 21)

#endif // UTIL_H
