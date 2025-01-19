#include "world.h"
#include "background.h"

static const int CBB_INDEX = 0;
static const int BG_DIM = 32; // background dimensions: 32x32 tiles

// convention is to make the screen base block as far right in VRAM as you can
// we're using a map of size 32x32 tiles, so we can use the very last one
static const int SBB_INDEX = 31;

void load_bgs() {
  // set the background using charblock 0 as the character base block
  // and SBB. each background tile is 4bpp. background is of size 32x32 tiles.
  REG_BG0CNT = BG_CBB(CBB_INDEX) | BG_SBB(SBB_INDEX) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  // load background data into memory
  memcpy32(pal_bg_mem, backgroundPal, backgroundPalLen / sizeof(u32));
  memcpy16(&tile_mem[CBB_INDEX][0], backgroundTiles,
           backgroundTilesLen / sizeof(u16));
}

void init_world(int floor_tile_y) {
  SCREENBLOCK *screen_block = &se_mem[SBB_INDEX];
  int floor_tile_start_index = floor_tile_y * BG_DIM;
  int amt_bg_tiles = backgroundTilesLen / sizeof(u16);
  for (int i = floor_tile_start_index; i < BG_DIM + floor_tile_start_index;
       i++) {
    for (u16 j = 0; j < amt_bg_tiles; j++) {
      *screen_block[i] = j;
    }
  }
}
