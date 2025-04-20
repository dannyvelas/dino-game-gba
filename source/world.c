#include "world.h"
#include "background.h"
#include "sprites.h"
#include "util.h"

// we'll use CBB #0 for BG 0 (text, the front-most bg)
// and CBB #1 for BG 1 (the floor, the back-most bg)
static const int BG0_CBB = 0;
static const int BG1_CBB = 1;

// we're using a map of size 32x32 screen-entries, (aka 32*32*2 => 2048 bytes)
// which is the same exact size as one screenblock
// convention is to make the screen base block as far right in VRAM as you can
static const int BG0_SBB = 30;
static const int BG1_SBB = 31;

// initialize tonc text engine (tte) into bg 0
void setupTTEBG0() {
  tte_init_se(0, // BG 0
              BG_CBB(BG0_CBB) | BG_SBB(BG0_SBB) | BG_4BPP |
                  BG_REG_32x32, // BG control (for REG_BGxCNT)
              0,                // Tile offset (special cattr)
              0x294a,           // Ink color
              0,                // BitUnpack offset
              NULL,             // Default font (sys8)
              NULL);            // Default renderer
  tte_write("\n Hello world! in gray\n");
}

void loadBG1() {
  // set the character base block and screenbase block of background 0.
  // each background tile is 4bpp. background is of size 32x32 tiles.
  REG_BG1CNT = BG_CBB(BG1_CBB) | BG_SBB(BG1_SBB) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG1HOFS = 0;
  REG_BG1VOFS = 0;

  // load background data into memory
  memcpy32(&tile_mem[BG1_CBB][0], backgroundTiles,
           backgroundTilesLen / sizeof(u32));
  memcpy16(pal_bg_bank, backgroundPal, backgroundPalLen / sizeof(u16));

  // initialize tiles
  int scr_entry_start = FLOOR_SCR_ENTRY_Y * BG_DIM;
  int amt_bg_tiles = backgroundTilesLen / sizeof(u32) / TILE_32BIT_AMT;
  for (int i = 0; i < BG_DIM * BG_DIM; i++) {
    // set floor tiles
    if (i >= scr_entry_start && i < BG_DIM + scr_entry_start) {
      u16 tile_index = (i % (amt_bg_tiles - 1)) + 1;
      se_mem[BG1_SBB][i] |= tile_index;
    }
  }
}

// load sprite data into memory at very first sprite charblock (4)
void loadSpriteData() {
  memcpy32(&tile_mem[4][0], spritesTiles, spritesTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, spritesPal, spritesPalLen / sizeof(u16));
}

void load_world() {
  setupTTEBG0();

  loadBG1();

  loadSpriteData();
}
