#ifndef CACTI_H
#define CACTI_H

#include "hitbox.h"
#include "obj_buffer.h"
#include <tonc.h>

// the first cactus is 4th in the sprite sheet
#define CACTI__START_SPRITE_INDEX 4
// there are two cacti in total
#define CACTI__AMT 2

struct cactus_state {
  OBJ_ATTR *cactus_obj;

  u32 tile_index;
  int x;
  int y;

  struct hitbox hitboxes[2];
};

struct cactus_state *init_cacti_state(struct buffer_state *);

#endif // CACTI_H
