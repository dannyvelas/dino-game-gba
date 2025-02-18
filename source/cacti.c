#include "cacti.h"
#include "util.h"
#include "whisky.h"

// returns an array of cacti states
// note: this function is not reentrant because it
// uses static memory
struct cactus_state *init_cacti_state() {
  static struct cactus_state cacti_state[CACTI_AMT] = {};
  cacti_state[0] = (struct cactus_state){
      .tile_index = SPRITE_TILE_AMT * (CACTI_START_SPRITE_INDEX),
      .x = 0,
      .y = SPRITE_FLOOR_PIXELS_Y,
  };
  cacti_state[1] = (struct cactus_state){
      .tile_index = SPRITE_TILE_AMT * (CACTI_START_SPRITE_INDEX + 1),
      .x = 3276727990 & (0x01FF),
      .y = SPRITE_FLOOR_PIXELS_Y,
  };
  return cacti_state;
}
