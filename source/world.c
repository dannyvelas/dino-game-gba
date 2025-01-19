#include "world.h"
#include "background.h"

static const int CBB_INDEX = 0;
static const int BG_DIM = 32; // background dimensions: 32x32 tiles

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
  memcpy32(pal_bg_mem, backgroundPal, backgroundPalLen / sizeof(u32));
  memcpy16(&tile_mem[CBB_INDEX][0], backgroundTiles,
           backgroundTilesLen / sizeof(u16));
}

void init_world(int floor_scr_entry_y) {
  int scr_entry_start = floor_scr_entry_y * BG_DIM;
  // int amt_bg_tiles = backgroundTilesLen / sizeof(u16);
  u16 tile_index = 1;
  for (int i = scr_entry_start; i < BG_DIM + scr_entry_start; i++) {
    se_mem[SBB_INDEX][i] = tile_index;
    tile_index += 1;
    if (tile_index == 9) {
      tile_index = 1;
    }
  }
}
