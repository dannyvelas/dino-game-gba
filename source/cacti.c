#include "cacti.h"
#include "util.h"
#include "whisky.h"

// returns an array of cacti states
// note: this function is not reentrant because it
// uses static memory
struct cactus_state *init_cacti_state() {
  static struct cactus_state cacti_state[CACTI_AMT] = {};
  for (int i = 0; i < CACTI_AMT; i++) {
    cacti_state[i] = (struct cactus_state){
        .tile_index = SPRITE_TILE_AMT * (CACTI_START_SPRITE_INDEX + i),
        .x = whisky1(i) & 0x01FF,
        .y = SPRITE_FLOOR_PIXELS_Y,
    };
  }
  return cacti_state;
}
