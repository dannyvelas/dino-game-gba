#include "dino.h"
#include "util.h"

struct dino_state init_dino_state(int start_y) {
  // some constants for calculating jumps
  int jump_speed = 4; // roughly pixels per frame
  int jump_height = jump_speed * 20;

  struct dino_state state = {
      .tile_index = 0,
      .palette_bank_index = 0,
      .start_y = start_y,
      .x = 0,
      .y = start_y,
      .direction = -1,
      .action = LEFT_STEP,
      .jump_speed = jump_speed,
      .jump_height = jump_height,
  };

  return state;
}

void update_dino_state(struct dino_state *state, int frame) {
  if (state->y == state->start_y && key_hit(KEY_A)) {
    // if our y coordinate is at start and A is hit, start a jump
    state->action = JUMPING;
    return;
  }

  if (state->action == JUMPING) {
    jump(state);
  } else if (frame % 6 == 0 && state->action == LEFT_STEP) {
    state->action = RIGHT_STEP;
  } else if (frame % 6 == 0 && state->action == RIGHT_STEP) {
    state->action = LEFT_STEP;
  }

  state->tile_index = state->action * SPRITE_TILE_DIM;
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
