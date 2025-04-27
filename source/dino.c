#include "dino.h"
#include "util.h"

// some constants for calculating jumps
const int jump_speed = 4; // roughly pixels per frame
const int jump_height = jump_speed * 20;

void reset_dino_state(struct dino_state *state) {
  state->alive = 1;
  state->tile_index = 0;
  state->palette_bank_index = 0;
  state->start_y = SPRITE_FLOOR_PIXELS_Y;
  state->x = 0;
  state->y = SPRITE_FLOOR_PIXELS_Y;
  state->direction = -1;
  state->action = LEFT_STEP;
  state->jump_speed = 4;
  state->jump_height = jump_height;

  // init default object values for our dino. we will copy this to OAM on VBLANK
  obj_set_attr(state->dino_obj, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(state->palette_bank_index) | state->tile_index);
}

struct dino_state init_dino_state(struct buffer_state *obj_buffer) {
  struct dino_state state = {
      .hitboxes =
          {// head hitbox
           {
               .left = 15,
               .right = 24,
               .top = 5,
               .bottom = 13,
           },
           // body hitbox
           {
               .left = 6,
               .right = 22,
               .top = 11,
               .bottom = 21,
           },
           // legs hitbox
           {
               .left = 10,
               .right = 17,
               .top = 15,
               .bottom = 26,
           }},
      .dino_obj = alloc_obj(obj_buffer),
  };

  reset_dino_state(&state);
  return state;
}

void update_dino_state(struct dino_state *state, int frame) {
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
  state->dino_obj->attr2 =
      ATTR2_BUILD(state->tile_index, state->palette_bank_index, 0);
  obj_set_pos(state->dino_obj, state->x, state->y);
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
