#include "cacti.h"
#include "dino.h"
#include "engine.h"
#include "obj_buffer.h"
#include "world.h"
#include <tonc.h>

int main() {
  // set I/O reg to use mode0, sprites, 1d sprites and tiled backgrounds 0 & 1
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;
  // put tonc interrupt switchboard at address that is used for interrupts
  irq_init(NULL);
  // make hardware fire and receive VBlank interrupts. No function will run;
  // this is just needed for VBlankIntrWait
  irq_add(II_VBLANK, NULL);

  // load background and sprite assets
  load_world();
  // also initialize the background on the screen
  // init_world();

  // initialize our object buffer that will be used for the rest of this program
  // struct buffer_state buffer_state = init_buffer();
  //// initialize dino state with a pointer to an object buffer
  // struct dino_state dino_state = init_dino_state(&buffer_state);
  //// init cacti
  // struct cactus_state *cacti_state = init_cacti_state(&buffer_state);

  // int frame = 0;
  // int scroll_velocity = 2;
  // int scroll_offset = 0;
  // while (1) {
  //   VBlankIntrWait();
  //   key_poll();

  //  // update OAM with new values that were calculated in last frame
  //  oam_copy(oam_mem, buffer_state.obj_buffer, buffer_state.len);
  //  // if out dinosaur crashed into something last iteration, lets break
  //  if (!dino_state.alive) {
  //    break;
  //  }

  //  // update dino state struct, and dino buffer
  //  update_dino_state(&dino_state, frame);

  //  // update cacti state struct, and cacti buffers
  //  update_cacti_state(cacti_state, scroll_velocity);

  //  // if collision happened, update dino state to be gameover
  //  if (detected_collision(dino_state, cacti_state)) {
  //    dino_state.alive = 0;
  //    update_dino_state(&dino_state, frame);
  //  }

  //  // scroll horizontal window
  //  scroll_offset += scroll_velocity;
  //  REG_BG0HOFS = scroll_offset;
  //  frame += 1;
  //}

  // game over in low power mode
  while (1) {
    Halt();
  }

  return 0;
}
