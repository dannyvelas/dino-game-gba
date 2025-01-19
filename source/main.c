#include "background.h"
#include "dino.h"
#include "dinosaur.h"
#include "util.h"
#include "world.h"
#include <tonc.h>

// memory index defines
// convention is to make the screen base block as far right in VRAM as you can
// we are using a map that is of size 32x32 tiles, so we can use the very last
// screen block
#define SBB_INDEX 31
#define CBB_INDEX 0

#define BG_DIM 32 // background dimensions: 32x32 tiles

int main() {
  // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

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

  // load sprite data into memory at very first sprite charblock (4)
  memcpy32(&tile_mem[4][0], dinosaurTiles, dinosaurTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, dinosaurPal, dinosaurPalLen / sizeof(u16));

  // init world
  int floor_tile_y = SCREEN_HEIGHT_T - 4; // lets make floor 4 tiles off ground
  init_world(se_mem[SBB_INDEX], floor_tile_y);

  // init dino state
  struct dino_state state = init_dino_state(tiles_to_pixels(floor_tile_y));

  int frame = 0;
  int scroll_velocity = 2;
  int scroll_offset = 0;
  while (1) {
    vid_vsync();
    key_poll();

    update_dino_state(&state, frame);

    // update OAM with new values that were calculated in this frame
    obj_set_attr(state.dino, ATTR0_SQUARE, ATTR1_SIZE_32,
                 ATTR2_PALBANK(state.palette_bank_index) |
                     state.start_tile_index);
    obj_set_pos(state.dino, state.x, state.y);
    oam_copy(oam_mem, state.dino, 1);

    // scroll horizontal window
    scroll_offset += scroll_velocity;
    REG_BG0HOFS = scroll_offset;
    frame += 1;
  }

  return 0;
}
