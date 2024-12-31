#include "dino.h"
#include "sky.h"
#include <tonc.h>

// convention is to make the screen base block as far right in VRAM as you can
// we are using a 32x32t map so we can use the very last screen block
#define SBB_INDEX 31
#define CBB_INDEX 0

#define TILE_HEIGHT 8
#define AMT_ROWS (SCREEN_HEIGHT / TILE_HEIGHT)
#define TILE_N 32

int main() {
  // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
  REG_DISPCNT = DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

  // set the background using charblock 0 as the character base block
  // and SBB. 4bpp background tiles and 32x32 tiles
  REG_BG0CNT = BG_CBB(CBB_INDEX) | BG_SBB(SBB_INDEX) | BG_4BPP | BG_REG_32x32;
  // set scrolling registers to 0
  REG_BG0HOFS = 0;
  REG_BG0VOFS = 0;

  // load background data into memory
  memcpy32(pal_bg_mem, skyPal, skyPalLen / sizeof(u32));
  memcpy16(&tile_mem[CBB_INDEX][0], skyTiles, skyTilesLen / sizeof(u16));

  // load background tile maps
  int floor_tile_y =
      (AMT_ROWS - 4); // lets make the floor 3 tiles up from bottom
  int floor_tile_index = 1;
  toncset16(&se_mem[SBB_INDEX][floor_tile_y * TILE_N], floor_tile_index,
            TILE_N);

  // place dino sprite into very first sprite charblock (4)
  memcpy32(&tile_mem[4][0], dinoTiles, dinoTilesLen / sizeof(u32));
  memcpy16(pal_obj_mem, dinoPal, dinoPalLen / sizeof(u16));

  // init buffer for this dino. we will copy this to OAM on VBLANK
  OBJ_ATTR dino = {};
  oam_init(&dino, 1);

  // initialize dino attributes
  // lets make our dinosaur be 2 tiles to the right and 4 tiles above the floor
  int x = TILE_HEIGHT * 2;
  int y = (floor_tile_y - 4) * TILE_HEIGHT;
  u32 tile_index = 0, palette_bank = 0;
  obj_set_attr(&dino, ATTR0_SQUARE, ATTR1_SIZE_32,
               ATTR2_PALBANK(palette_bank) | tile_index);

  // set initial position of this dino
  obj_set_pos(&dino, x, y);
  oam_copy(oam_mem, &dino, 1);

  int offset = 0;
  int direction = -1;
  int jump_initiated = 0;
  while (1) {
    vid_vsync();
    key_poll();
    // if we're static and A is hit, start a jump
    if (offset == 0 && key_hit(KEY_A)) {
      jump_initiated = 1;
      continue;
    }

    if (direction == -1 && offset == -5) {
      // if we reached the arc of our jump, start going down
      direction = 1;
    } else if (offset == 0 && direction == 1) {
      // if we reached the floor after a jump, flip the direction
      // so that when we jump we go up again
      direction = -1;
      jump_initiated = 0;
    } else if ((direction == -1 && -4 <= offset && offset <= 0 &&
                jump_initiated) ||
               (direction == 1 && -5 <= offset && offset <= -1)) {
      // if we're in the middle of going up or going down in a jump
      // continue moving in that direction
      offset += 1 * direction;
      y += TILE_HEIGHT * direction;
      obj_set_pos(&dino, x, y);
      oam_copy(oam_mem, &dino, 1);
    }
  }

  while (1) {
    vid_vsync();
    x += 1;
    obj_set_pos(&dino, x, y);
    oam_copy(oam_mem, &dino, 1);
  }

  return 0;
}
