#include "dino.h"
#include "util.h"
#include "world.h"
#include <tonc.h>

int main() {
  // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;
  // put tonc interrupt switchboard at address that is used for interrupts
  irq_init(NULL);
  // make hardware fire and receive VBlank interrupts. No function will run;
  // this is just needed for VBlankIntrWait
  irq_add(II_VBLANK, NULL);

  // load background and sprite assets
  load_world();

  // init world; lets make floor 4 screenblock entries off ground
  int floor_scr_entry_y = SCREEN_HEIGHT_T - 4;
  init_world(floor_scr_entry_y);

  // init dino state
  struct dino_state state = init_dino_state(tiles_to_pixels(floor_scr_entry_y));

  int frame = 0;
  int scroll_velocity = 2;
  int scroll_offset = 0;
  while (1) {
    VBlankIntrWait();
    key_poll();

    update_dino_state(&state, frame);

    // update OAM with new values that were calculated in this frame
    state.dino->attr2 =
        ATTR2_BUILD(state.tile_index, state.palette_bank_index, 0);
    obj_set_pos(state.dino, state.x, state.y);
    oam_copy(oam_mem, state.dino, 1);

    // scroll horizontal window
    scroll_offset += scroll_velocity;
    REG_BG0HOFS = scroll_offset;
    frame += 1;
  }

  return 0;
}
