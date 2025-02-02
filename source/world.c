#include "world.h"
#include "background.h"
#include "sprites.h"
#include "util.h"

static const int CBB_INDEX = 0;
static const int BG_DIM = 32;     // background dimensions: 32x32 tiles
static const int TILE_LEN_32 = 8; // a group of 8 32-bit ints makes one tile
static const int CACTUS_SPRITE_INDEX = 3; // cactus is 4th in the sprite sheet

// convention is to make the screen base block as far right in VRAM as you can
// we're using a map of size 32x32 tiles, so we can use the very last one
static const int SBB_INDEX = 31;

void load_world() {
  // set the background using charblock 0 as the character base block
  // and SBB. each background tile is 4bpp. background is of size 32x32 tiles.
  REG_BG0CNT = BG_CBB(CBB_INDEX) | BG_SBB(SBB_INDEX) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  // load background data into memory
  memcpy32(&tile_mem[CBB_INDEX][0], backgroundTiles,
           backgroundTilesLen / sizeof(u32));
  memcpy16(pal_bg_mem, backgroundPal, backgroundPalLen / sizeof(u16));

  // load sprite data into memory at very first sprite charblock (4)
  memcpy32(&tile_mem[4][0], spritesTiles, spritesTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, spritesPal, spritesPalLen / sizeof(u16));
}

void init_floor(int floor_scr_entry_y) {
  int scr_entry_start = floor_scr_entry_y * BG_DIM;
  int amt_bg_tiles = backgroundTilesLen / sizeof(u32) / TILE_LEN_32;
  for (int i = scr_entry_start; i < BG_DIM + scr_entry_start; i++) {
    u16 tile_index = (i % (amt_bg_tiles - 1)) + 1;
    se_mem[SBB_INDEX][i] = tile_index;
  }
}

void init_cacti(OBJ_ATTR *obj_buffer, int floor_pixels_y) {
  OBJ_ATTR *cactus = &obj_buffer[1];
  u32 tile_index = SPRITE_TILE_DIM * CACTUS_SPRITE_INDEX;
  obj_set_attr(cactus, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(0) | tile_index);
  obj_set_pos(cactus, 28, floor_pixels_y);
}

// initializes floor tiles and non-dino sprites
// returns the y pixel that sprites should use as a floor
int init_world(OBJ_ATTR *obj_buffer) {
  // lets make floor 4 screenblock entries off ground
  int floor_scr_entry_y = SCREEN_HEIGHT_T - 4;
  // lets make our sprites be 21 pixels above the floor
  int sprite_floor_pixels_y = tiles_to_pixels(floor_scr_entry_y) - 21;

  // init floor tiles
  init_floor(floor_scr_entry_y);

  // init cacti sprites
  init_cacti(obj_buffer, sprite_floor_pixels_y);

  return sprite_floor_pixels_y;
}
