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

  // these fields delineate where exactly our dinosaur resides inside of the
  // 32x32 sprite
  const int x_start;
  const int x_end;
  const int y_start;
  const int y_end;
};

struct dino_state init_dino_state();

void update_dino_state(struct dino_state *state, int frame);

void jump(struct dino_state *state);

#endif // DINO_H
