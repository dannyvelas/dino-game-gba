#include "dino.h"
#include <tonc.h>

#define TILE_DIM_PIXELS 8 // dimensions of tiles in terms of pixels
// dimensions of our dino sprite in terms of tiles
#define DINO_DIM_TILES 32
// amount of tiles in our dino sprite
#define DINO_AMT_TILES                                                         \
  ((DINO_DIM_TILES * DINO_DIM_TILES) / (TILE_DIM_PIXELS * TILE_DIM_PIXELS))

static void jump(struct dino_state *state) {
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

struct dino_state init_dino_state(int floor_tile_y) {
  // lets make our dino be 2 tiles to the right and 24 pixels above the floor
  int floor_y = TILE_DIM_PIXELS * floor_tile_y;
  int start_y = floor_y - 24;

  // some constants for calculating jumps
  int jump_speed = TILE_DIM_PIXELS / 2;
  int jump_height = jump_speed * 20;

  struct dino_state state = {
      .dino = {},
      .start_tile_index = 0,
      .palette_bank_index = 0,
      .start_y = start_y,
      .x = TILE_DIM_PIXELS * 2,
      .y = start_y,
      .direction = -1,
      .action = LEFT_STEP,
      .jump_speed = jump_speed,
      .jump_height = jump_height,
  };
  // init default object values for our dino. we will copy this to OAM on VBLANK
  oam_init(state.dino, 1);
  obj_set_attr(state.dino, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(state.start_tile_index) |
                   state.palette_bank_index);

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

  state->start_tile_index = state->action * DINO_AMT_TILES;
}
