#include "world.h"
#include "background.h"
#include "sprites.h"

static const int TTE_CBB = 0;
static const int BG0_CBB = 1;

// we're using a map of size 32x32 screen-entries, (aka 32*32*2 => 2048 bytes)
// which is the same exact size as one screenblock
// convention is to make the screen base block as far right in VRAM as you can
static const int TTE_SBB = 30;
static const int BG0_SBB = 31;

void loadBG0() {
  // set the character base block and screenbase block of background 0.
  // each background tile is 4bpp. background is of size 32x32 tiles.
  REG_BG0CNT = BG_CBB(BG0_CBB) | BG_SBB(BG0_SBB) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  // load background data into memory
  memcpy32(&tile_mem[BG0_CBB][0], backgroundTiles,
           backgroundTilesLen / sizeof(u32));
  memcpy16(pal_bg_bank, backgroundPal, backgroundPalLen / sizeof(u16));

  // initialize screen-map entries
  memcpy32(&se_mem[BG0_SBB][0], backgroundMap, backgroundMapLen / sizeof(u32));
}

// load sprite data into memory at very first sprite charblock (4)
void loadSpriteData() {
  memcpy32(&tile_mem[4][0], spritesTiles, spritesTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, spritesPal, spritesPalLen / sizeof(u16));
}

void initTTE() {
  tte_init_se(0, // Background number (BG 0)
              BG_CBB(TTE_CBB) | BG_SBB(TTE_SBB) | BG_4BPP |
                  BG_REG_32x32, // BG control (for REG_BGxCNT)
              0,                // Tile offset (special cattr)
              0x294A,           // Ink color
              0,                // BitUnpack offset (on-pixel = 15)
              NULL,             // Default font (sys8)
              NULL);            // Default renderer (se_drawg_s)
}

void load_world() {
  initTTE();

  loadBG0();

  loadSpriteData();
}
