#include "cacti.h"
#include "dino.h"
#include "obj_buffer.h"
#include "world.h"
#include <tonc.h>

int cactus_left_lt_dino_right(int cactus_left, int cactus_right,
                              int dino_right) {
  // if cactus left overflowed (outside frame), return true at all times
  if (cactus_left > cactus_right)
    return true;
  // otherwise, just return normal operation
  return cactus_left < dino_right;
}

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
  // also initialize the background on the screen
  init_world();

  // initialize our object buffer that will be used for the rest of this program
  struct buffer_state buffer_state = init_buffer();
  // initialize dino state with a pointer to an object buffer
  struct dino_state dino_state = init_dino_state(&buffer_state);
  // init cacti
  struct cactus_state *cacti_state = init_cacti_state(&buffer_state);

  int frame = 0;
  int scroll_velocity = 2;
  int scroll_offset = 0;
  while (1) {
    VBlankIntrWait();
    key_poll();

    // update OAM with new values that were calculated in last frame
    oam_copy(oam_mem, buffer_state.obj_buffer, buffer_state.len);

    // update dino state struct, and dino buffer
    update_dino_state(&dino_state, frame);
    if (!dino_state.alive) {
      continue;
    }

    // update cacti state structs and buffer
    for (int i = 0; i < CACTI__AMT; i++) {
      cacti_state[i].x = (cacti_state[i].x - scroll_velocity) & 0x01FF;
      obj_set_pos(cacti_state[i].cactus_obj, cacti_state[i].x,
                  cacti_state[i].y);

      // check if dino hit me
      for (int j = 0; j < DINO__HITBOX_AMT; j++) {
        int dino_left = dino_state.hitboxes[j].left;
        int dino_right = dino_state.hitboxes[j].right;
        int dino_bottom = dino_state.y + dino_state.hitboxes[j].bottom;

        for (int k = 0; k < CACTI__HITBOX_AMT; k++) {
          int cactus_left =
              (cacti_state[i].x + cacti_state[i].hitboxes[k].left) & 0x01FF;
          int cactus_top = cacti_state[i].y + cacti_state[i].hitboxes[k].top;
          int cactus_right =
              (cacti_state[i].x + cacti_state[i].hitboxes[k].right) & 0x01FF;

          if ((cactus_left_lt_dino_right(cactus_left, cactus_right,
                                         dino_right)) &&
              (cactus_right >= dino_left) && (dino_bottom >= cactus_top)) {
            dino_state.alive = 0;
          }
        }
      }
    }

    // scroll horizontal window
    scroll_offset += scroll_velocity;
    REG_BG0HOFS = scroll_offset;
    frame += 1;
  }

  return 0;
}
