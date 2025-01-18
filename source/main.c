#include "background.h"
#include "dinosaur.h"
#include <tonc.h>

// memory index defines
// convention is to make the screen base block as far right in VRAM as you can
// we are using a 32x32t map so we can use the very last screen block
#define SBB_INDEX 31
#define CBB_INDEX 0

// background defines
#define TILE_DIM_PIXELS 8 // dimensions of tiles in terms of pixels
#define AMT_ROWS                                                               \
  (SCREEN_HEIGHT / TILE_DIM_PIXELS) // amt of rows in the background
#define BG_DIM_TILES 32 // dimensions of background in terms of tiles

// sprite defines
// dimensions of our dino sprite in terms of tiles
#define DINO_DIM_TILES 32
// amount of tiles in our dino sprite
#define DINO_AMT_TILES                                                         \
  ((DINO_DIM_TILES * DINO_DIM_TILES) / (TILE_DIM_PIXELS * TILE_DIM_PIXELS))

enum dino_action { JUMPING, LEFT_STEP, RIGHT_STEP };

struct state {
  /* oam_init requires that this field is big enough for for one OBJ_AFFINE */
  OBJ_ATTR dino[4];
  int x;
  int y;
  int direction;
  enum dino_action action;
};

void jump(struct state *dino_state, int start_y) {
  int offset = dino_state->y - start_y;
  int jump_speed = TILE_DIM_PIXELS / 2;
  int arc = -(jump_speed * 20);
  if (dino_state->direction == -1 && offset == arc) {
    // if we reached the arc of our jump, start going down
    dino_state->direction = 1;
  } else if (offset == 0 && dino_state->direction == 1) {
    // if we reached the floor after a jump, flip the direction
    // so that when we jump we go up again
    dino_state->direction = -1;
    dino_state->action = LEFT_STEP;
  } else if ((dino_state->direction == -1 && offset <= 0) ||
             (dino_state->direction == 1 && offset <= -jump_speed)) {
    // if we're in the middle of going up or going down in a jump
    // continue moving in that direction
    dino_state->y += jump_speed * dino_state->direction;
  }
}

int main() {
  // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

  // set the background using charblock 0 as the character base block
  // and SBB. each background tile is 4bpp. background is of size 32x32 tiles.
  REG_BG0CNT = BG_CBB(CBB_INDEX) | BG_SBB(SBB_INDEX) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  // load background data into memory
  memcpy32(pal_bg_mem, backgroundPal, backgroundPalLen / sizeof(u32));
  memcpy16(&tile_mem[CBB_INDEX][0], backgroundTiles,
           backgroundTilesLen / sizeof(u16));

  // load sprite data into memory at very first sprite charblock (4)
  memcpy32(&tile_mem[4][0], dinosaurTiles, dinosaurTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, dinosaurPal, dinosaurPalLen / sizeof(u16));

  // load background tile maps. lets make the floor 3 tiles up from bottom
  int floor_tile_y = (AMT_ROWS - 4);
  int floor_tile_index = 1;
  toncset16(&se_mem[SBB_INDEX][floor_tile_y * BG_DIM_TILES], floor_tile_index,
            BG_DIM_TILES);

  // set initial state of our dino
  // lets make our dinosaur be 24 pixels above the floor
  int start_x = TILE_DIM_PIXELS * 2;
  int floor_y = TILE_DIM_PIXELS * floor_tile_y;
  int start_y = floor_y - 24;
  struct state dino_state = {
      .dino = {},
      .x = start_x,
      .y = start_y,
      .direction = -1,
      .action = LEFT_STEP,
  };
  // init default object values for our dino. we will copy this to OAM on VBLANK
  oam_init(dino_state.dino, 1);
  u32 sprite_start_tile = 0, palette_bank = 0;
  obj_set_attr(dino_state.dino, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(palette_bank) | sprite_start_tile);

  u16 i = 0;
  while (1) {
    vid_vsync();
    key_poll();
    if (dino_state.y == start_y && key_hit(KEY_A)) {
      // if our y coordinate is at start and A is hit, start a jump
      dino_state.action = JUMPING;
      continue;
    }

    if (dino_state.action == JUMPING) {
      jump(&dino_state, start_y);
    } else if (i % 6 == 0 && dino_state.action == LEFT_STEP) {
      dino_state.action = RIGHT_STEP;
    } else if (i % 6 == 0 && dino_state.action == RIGHT_STEP) {
      dino_state.action = LEFT_STEP;
    }

    sprite_start_tile = dino_state.action * DINO_AMT_TILES;
    obj_set_attr(dino_state.dino, ATTR0_SQUARE, ATTR1_SIZE_32,
                 ATTR2_PALBANK(palette_bank) | sprite_start_tile);

    // update OAM with new values that were calculated in this frame
    obj_set_pos(dino_state.dino, dino_state.x, dino_state.y);
    oam_copy(oam_mem, dino_state.dino, 1);

    // scroll horizontal window
    REG_BG0HOFS = i * 2;
    i += 1;
  }

  return 0;
}
