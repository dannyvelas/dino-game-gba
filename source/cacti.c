#include "cacti.h"
#include "util.h"

// returns an array of cacti states
// note: this function is not reentrant because it
// uses static memory
struct cactus_state *init_cacti_state() {
  static struct cactus_state cacti_state[CACTI_AMT] = {{
      .tile_index = SPRITE_TILE_AMT * CACTI_START_SPRITE_INDEX,
      .x = 124,
      .y = SPRITE_FLOOR_PIXELS_Y,
  }};
  return cacti_state;
}
