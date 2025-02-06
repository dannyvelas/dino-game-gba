#ifndef CACTI_H
#define CACTI_H

#include <tonc.h>

// the first cactus is 4th in the sprite sheet
#define CACTI_START_SPRITE_INDEX 3
// there are two cacti in total
#define CACTI_AMT 2

struct cactus_state {
  u32 tile_index;
  int x;
  int y;
};

struct cactus_state *init_cacti_state();

#endif // CACTI_H
