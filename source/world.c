#include "world.h"
#include "background.h"
#include "sprites.h"
#include "util.h"

// we'll use CBB #1 for generic background tiles in the deepest background (BG0)
// and CBB #0 for text, one background above (BG1)
static const int BG0_CBB = 1;
static const int BG1_CBB = 0;

// we're using a map of size 32x32 screen-entries, (aka 32*32*2 => 2048 bytes)
// which is the same exact size as one screenblock entry
// convention is to make the screen base block as far right in VRAM as you can,
// so we'll use the last one
static const int BG0_SBB = 31;
static const int BG1_SBB = 30;

// we'll use CBB #1 for generic background tiles in the deepest background (BG0)
// and CBB #0 for text, one background above (BG1)
static const int BG0_PALBANK = 1;
static const int BG1_PALBANK = 0;

void configureBG0() {
  // set the character base block and screenbase block of background 0.
  // each background tile is 4bpp. background is of size 32x32 tiles.
  REG_BG0CNT = BG_CBB(BG0_CBB) | BG_SBB(BG0_SBB) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;
}

// initialize tonc text engine (tte) into bg 1
void setupTTEBG1() {
  tte_init_se(1,                                 // BG 1
              BG_CBB(BG1_CBB) | BG_SBB(BG1_SBB), // BG control (for REG_BGxCNT)
              0,                                 // Tile offset (special cattr)
              CLR_YELLOW,                        // Ink color
              0,                                 // BitUnpack offset
              NULL,                              // Default font (sys8)
              NULL);                             // Default renderer
  tte_write("\n Hello world! in yellow\n");
}

// load background data into memory
void loadBGData() {
  memcpy32(&tile_mem[BG0_CBB][0], backgroundTiles,
           backgroundTilesLen / sizeof(u32));
  memcpy16(pal_bg_bank[1], backgroundPal, backgroundPalLen / sizeof(u16));
}

// load sprite data into memory at very first sprite charblock (4)
void loadSpriteData() {
  memcpy32(&tile_mem[4][0], spritesTiles, spritesTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, spritesPal, spritesPalLen / sizeof(u16));
}

void load_world() {
  configureBG0();

  setupTTEBG1();

  loadBGData();

  loadSpriteData();

  init_world();
}

// initializes floor tiles
void init_world() {
  int scr_entry_start = FLOOR_SCR_ENTRY_Y * BG_DIM;
  int amt_bg_tiles = backgroundTilesLen / sizeof(u32) / TILE_32BIT_AMT;

  for (int i = 0; i < BG_DIM * BG_DIM; i++) {
    // set floor tiles
    if (i >= scr_entry_start && i < BG_DIM + scr_entry_start) {
      u16 tile_index = (i % (amt_bg_tiles - 1)) + 1;
      se_mem[BG0_SBB][i] = 0x1000 | tile_index;
      continue;
    }

    // set background tiles
    se_mem[BG0_SBB][i] = 0x1000;
  }

  for (int i = scr_entry_start; i < BG_DIM + scr_entry_start; i++) {
  }
}
