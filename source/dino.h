#ifndef DINO_H
#define DINO_H

#include "hitbox.h"
#include "obj_buffer.h"
#include <tonc.h>

#define DINO__HITBOX_AMT 3

enum dino_action { JUMPING, LEFT_STEP, RIGHT_STEP, GAMEOVER };

struct dino_state {
  OBJ_ATTR *dino_obj;
  int alive;
  u32 tile_index;
  u32 palette_bank_index;
  int start_y;
  int x;
  int y;
  int direction;
  enum dino_action action;
  const int jump_speed;
  const int jump_height;

  // we will have three discrete hitboxes for this sprite
  // each value is a coordinate based on the top-left pixel of this 32x32 sprite
  struct hitbox hitboxes[DINO__HITBOX_AMT];
};

// initializes dino state with a pointer to the dino object buffer
struct dino_state init_dino_state(struct buffer_state *);

void update_dino_state(struct dino_state *state, int frame);

void jump(struct dino_state *state);

#endif // DINO_H
