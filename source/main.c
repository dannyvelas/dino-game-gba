#include "dino.h"
#include "dinosaur.h"
#include "util.h"
#include "world.h"
#include <tonc.h>

int main() {
  // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

  // load sprite data into memory at very first sprite charblock (4)
  memcpy32(&tile_mem[4][0], dinosaurTiles, dinosaurTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, dinosaurPal, dinosaurPalLen / sizeof(u16));

  // init world; lets make floor 4 screenblock entries off ground
  load_world();
  int floor_scr_entry_y = SCREEN_HEIGHT_T - 4;
  init_world(floor_scr_entry_y);

  // init dino state
  struct dino_state state = init_dino_state(tiles_to_pixels(floor_scr_entry_y));

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
