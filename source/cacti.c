#include "cacti.h"
#include "util.h"
#include "whisky.h"

// this is the same as whisky1 except it provides some conveniances such as
// allowing for a seed of 0 and making sure the result is only 9 bits (aka
// the amount of bits allowed for an x coordinate in OAM)
int generate_random_coord(int seed) { return whisky1(seed + 1) & 0x01FF; }

// returns an array of cacti states
// note: this function is not reentrant because it
// uses static memory
struct cactus_state *init_cacti_state() {
  // initialize cacti with sprite metadata
  static struct cactus_state cacti_state[CACTI__AMT] = {
      {.start_pixel = 10, .end_pixel = 21},
      {.start_pixel = 2, .end_pixel = 29},
  };

  // set remaining values
  for (int i = 0; i < CACTI__AMT; i++) {
    cacti_state[i].tile_index =
        SPRITE_TILE_AMT * (CACTI__START_SPRITE_INDEX + i);
    cacti_state[i].x = generate_random_coord(i);
    cacti_state[i].y = SPRITE_FLOOR_PIXELS_Y;
  }
  return cacti_state;
}
