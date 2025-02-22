#ifndef ENGINE_H
#define ENGINE_H

#include "cacti.h"
#include "dino.h"

int cactus_left_lt_dino_right(int cactus_left, int cactus_right,
                              int dino_right);

int detect_collision(struct dino_state dino_state,
                     struct cactus_state *cacti_state);

#endif // ENGINE_H
