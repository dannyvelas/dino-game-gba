#include "world.h"
#include "background.h"
#include "sprites.h"
#include "util.h"

static const int CBB_INDEX = 0;
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

// initializes floor tiles and non-dino sprites
void init_world() {
  int scr_entry_start = FLOOR_SCR_ENTRY_Y * BG_DIM;
  int amt_bg_tiles = backgroundTilesLen / sizeof(u32) / TILE_32BIT_AMT;
  for (int i = scr_entry_start; i < BG_DIM + scr_entry_start; i++) {
    u16 tile_index = (i % (amt_bg_tiles - 1)) + 1;
    se_mem[SBB_INDEX][i] = tile_index;
  }
}
