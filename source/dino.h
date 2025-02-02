#ifndef DINO_H
#define DINO_H

#include <tonc.h>

enum dino_action { JUMPING, LEFT_STEP, RIGHT_STEP };

struct dino_state {
  u32 tile_index;
  u32 palette_bank_index;
  int start_y;
  int x;
  int y;
  int direction;
  enum dino_action action;
  const int jump_speed;
  const int jump_height;
};

struct dino_state init_dino_state(int floor_tile_y);

void update_dino_state(struct dino_state *state, int frame);

void jump(struct dino_state *state);

#endif // DINO_H
