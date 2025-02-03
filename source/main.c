#include "dino.h"
#include "world.h"
#include <tonc.h>

// buffer to store sprites
OBJ_ATTR obj_buffer[128];

struct cactus_state {
  u32 tile_index;
  int x;
  int y;
};

int main() {
  // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;
  // put tonc interrupt switchboard at address that is used for interrupts
  irq_init(NULL);
  // make hardware fire and receive VBlank interrupts. No function will run;
  // this is just needed for VBlankIntrWait
  irq_add(II_VBLANK, NULL);
  // initialize our object buffer that will be used for the rest of this program
  oam_init(obj_buffer, 128);

  // load background and sprite assets
  load_world();

  // init world
  int sprite_floor_pixels_y = init_world(obj_buffer);

  // initialize dino sprite and state
  OBJ_ATTR *dino = &obj_buffer[0];
  struct dino_state state = init_dino_state(sprite_floor_pixels_y);
  // init default object values for our dino. we will copy this to OAM on VBLANK
  obj_set_attr(dino, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(state.palette_bank_index) | state.tile_index);

  // init cactus
  OBJ_ATTR *cactus = &obj_buffer[1];
  struct cactus_state cactus_state = {48, 124, sprite_floor_pixels_y};
  obj_set_attr(cactus, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(0) | cactus_state.tile_index);

  int frame = 0;
  int scroll_velocity = 2;
  int scroll_offset = 0;
  while (1) {
    VBlankIntrWait();
    key_poll();

    // update dino state struct, and dino buffer
    update_dino_state(&state, frame);
    dino->attr2 = ATTR2_BUILD(state.tile_index, state.palette_bank_index, 0);
    obj_set_pos(dino, state.x, state.y);

    // update cactus state struct and buffer
    cactus_state.x -= scroll_velocity;
    obj_set_pos(cactus, cactus_state.x, cactus_state.y);

    // update OAM with new values that were calculated in this frame
    oam_copy(oam_mem, obj_buffer, 2);

    // scroll horizontal window
    scroll_offset += scroll_velocity;
    REG_BG0HOFS = scroll_offset;
    frame += 1;
  }

  return 0;
}
