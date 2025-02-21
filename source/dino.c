#include "dino.h"
#include "util.h"

struct dino_state init_dino_state() {
  // some constants for calculating jumps
  int jump_speed = 4; // roughly pixels per frame
  int jump_height = jump_speed * 20;

  // some sprite metadata
  int x_start = 6; // 6 pixels of padding on left in this 32x32 sprite
  int x_end = 24;  // 7 pixels of padding on right
  int y_start = 5; // 5 pixels of padding on top
  int y_end = 26;  // 5 pixels of padding on bottom

  struct dino_state state = {
      .alive = 1,
      .tile_index = 0,
      .palette_bank_index = 0,
      .start_y = SPRITE_FLOOR_PIXELS_Y,
      .x = 0,
      .y = SPRITE_FLOOR_PIXELS_Y,
      .direction = -1,
      .x_start = x_start,
      .x_end = x_end,
      .y_start = y_start,
      .y_end = y_end,
      .action = LEFT_STEP,
      .jump_speed = jump_speed,
      .jump_height = jump_height,
  };

  return state;
}

void update_dino_state(struct dino_state *state, OBJ_ATTR *dino, int frame) {
  if (!state->alive) {
    state->action = GAMEOVER;
  } else if (state->y == state->start_y && key_hit(KEY_A)) {
    state->action = JUMPING;
  } else if (state->action == JUMPING) {
    jump(state);
  } else if (frame % 6 == 0 && state->action == LEFT_STEP) {
    state->action = RIGHT_STEP;
  } else if (frame % 6 == 0 && state->action == RIGHT_STEP) {
    state->action = LEFT_STEP;
  }

  state->tile_index = state->action * SPRITE_TILE_AMT;

  // update object buffer
  dino->attr2 = ATTR2_BUILD(state->tile_index, state->palette_bank_index, 0);
  obj_set_pos(dino, state->x, state->y);
}

void jump(struct dino_state *state) {
  int offset = state->y - state->start_y;
  if (state->direction == -1 && offset == -state->jump_height) {
    // if we reached the arc of our jump, start going down
    state->direction = 1;
  } else if (offset == 0 && state->direction == 1) {
    // if we reached the floor after a jump, flip the direction
    // so that when we jump we go up again
    state->direction = -1;
    state->action = LEFT_STEP;
  } else if ((state->direction == -1 && offset <= 0) ||
             (state->direction == 1 && offset <= -state->jump_speed)) {
    // if we're in the middle of going up or going down in a jump
    // continue moving in that direction
    state->y += state->jump_speed * state->direction;
  }
}
