#include "engine.h"

int cactus_left_lt_dino_right(int cactus_left, int cactus_right,
                              int dino_right) {
  // if cactus left overflowed (outside frame), return true at all times
  if (cactus_left > cactus_right)
    return true;
  // otherwise, just return normal operation
  return cactus_left < dino_right;
}

int detect_collision(struct dino_state dino_state,
                     struct cactus_state *cacti_state) {
  for (int i = 0; i < DINO__HITBOX_AMT; i++) {
    int dino_left = dino_state.hitboxes[i].left;
    int dino_right = dino_state.hitboxes[i].right;
    int dino_bottom = dino_state.y + dino_state.hitboxes[i].bottom;

    for (int j = 0; j < CACTI__AMT; j++) {
      struct cactus_state cactus_state = cacti_state[j];
      for (int k = 0; k < CACTI__HITBOX_AMT; k++) {
        int cactus_left =
            (cactus_state.x + cactus_state.hitboxes[k].left) & 0x01FF;
        int cactus_top = cactus_state.y + cactus_state.hitboxes[k].top;
        int cactus_right =
            (cactus_state.x + cactus_state.hitboxes[k].right) & 0x01FF;

        if ((cactus_left_lt_dino_right(cactus_left, cactus_right,
                                       dino_right)) &&
            (cactus_right >= dino_left) && (dino_bottom >= cactus_top)) {
          return true;
        }
      }
    }
  }
  return false;
}
