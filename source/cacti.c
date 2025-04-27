#include "cacti.h"
#include "util.h"

void reset_cacti_state(struct cactus_state *cacti_state, int seed) {
  for (int i = 0; i < CACTI__AMT; i++) {
    // set struct values
    cacti_state[i].tile_index =
        SPRITE_TILE_AMT * (CACTI__START_SPRITE_INDEX + i);
    cacti_state[i].x = (seed + i) & 0x01FF;
    cacti_state[i].y = SPRITE_FLOOR_PIXELS_Y;

    // set object values
    obj_set_attr(cacti_state[i].cactus_obj, ATTR0_SQUARE, ATTR1_SIZE_32,
                 ATTR2_PALBANK(0) | cacti_state[i].tile_index);
  }
}

// returns an array of cacti states
// note: this function is not reentrant because it
// uses static memory
struct cactus_state *init_cacti_state(struct buffer_state *buffer_state,
                                      int seed) {
  // initialize cacti with hitboxes
  static struct cactus_state cacti_state[CACTI__AMT] = {
      {.hitboxes = {{
                        .left = 13,
                        .top = 2,
                        .right = 17,
                        .bottom = 25,
                    },
                    {
                        .left = 9,
                        .top = 9,
                        .right = 22,
                        .bottom = 18,
                    }}},
      {.hitboxes = {{
                        .left = 6,
                        .top = 10,
                        .right = 26,
                        .bottom = 13,
                    },
                    {
                        .left = 3,
                        .top = 14,
                        .right = 30,
                        .bottom = 29,
                    }}},
  };

  for (int i = 0; i < CACTI__AMT; i++) {
    cacti_state[i].cactus_obj = alloc_obj(buffer_state);
  }

  reset_cacti_state(cacti_state, seed);
  return cacti_state;
}

void update_cacti_state(struct cactus_state *cacti_state, int scroll_velocity) {
  // update cacti state structs and buffer
  for (int i = 0; i < CACTI__AMT; i++) {
    cacti_state[i].x = (cacti_state[i].x - scroll_velocity) & 0x01FF;
    obj_set_pos(cacti_state[i].cactus_obj, cacti_state[i].x, cacti_state[i].y);
  }
}
