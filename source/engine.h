#ifndef ENGINE_H
#define ENGINE_H

#include "cacti.h"
#include "dino.h"

int detected_collision(struct dino_state dino_state,
                       struct cactus_state *cacti_state);

void render_gameover();

#endif // ENGINE_H
