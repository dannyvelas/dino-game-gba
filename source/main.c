#include "cacti.h"
#include "dino.h"
#include "world.h"
#include <tonc.h>

// buffer to store sprites
OBJ_ATTR obj_buffer[128];

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
  // also initialize the background on the screen
  init_world();

  // initialize dino sprite and state
  OBJ_ATTR *dino = &obj_buffer[0];
  struct dino_state dino_state = init_dino_state();
  // init default object values for our dino. we will copy this to OAM on VBLANK
  obj_set_attr(dino, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(dino_state.palette_bank_index) |
                   dino_state.tile_index);

  // init cacti
  struct cactus_state *cacti_state = init_cacti_state();
  for (int i = 0; i < CACTI__AMT; i++) {
    obj_set_attr(&obj_buffer[i + 1], ATTR0_SQUARE, ATTR1_SIZE_32,
                 ATTR2_PALBANK(0) | cacti_state[i].tile_index);
  }

  int frame = 0;
  int scroll_velocity = 2;
  int scroll_offset = 0;
  int alive = 1;
  while (1) {
    VBlankIntrWait();
    key_poll();

    // update dino state struct, and dino buffer
    update_dino_state(&dino_state, alive, frame);

    if (!alive) {
      goto gameover;
    }

    dino->attr2 =
        ATTR2_BUILD(dino_state.tile_index, dino_state.palette_bank_index, 0);
    obj_set_pos(dino, dino_state.x, dino_state.y);

    // update cacti state structs and buffer
    for (int i = 0; i < CACTI__AMT; i++) {
      cacti_state[i].x = (cacti_state[i].x - scroll_velocity) & 0x01FF;
      obj_set_pos(&obj_buffer[i + 1], cacti_state[i].x, cacti_state[i].y);

      // check if dino hit me
      int x_cacti_start = cacti_state[i].x + cacti_state[i].x_start;
      int x_cacti_end = cacti_state[i].x + cacti_state[i].x_end;
      int x_dino_start = dino_state.x + dino_state.x_start;
      int x_dino_end = dino_state.x + dino_state.x_end;

      int y_cacti_start = cacti_state[i].y + cacti_state[i].y_start;
      int y_dino_end = dino_state.y + dino_state.y_end;

      if ((x_cacti_start <= x_dino_end) && (x_cacti_end >= x_dino_start) &&
          (y_dino_end >= y_cacti_start)) {
        alive = 0;
      }
    }

    // update OAM with new values that were calculated in this frame
    oam_copy(oam_mem, obj_buffer, CACTI__AMT + 1);

    // scroll horizontal window
    scroll_offset += scroll_velocity;
    REG_BG0HOFS = scroll_offset;
    frame += 1;
  }

gameover:
  while (1)
    ;

  return 0;
}
