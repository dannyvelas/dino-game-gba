#include <string.h>
#include <tonc.h>
#include "dino.h"

OBJ_ATTR obj_buffer[128];

int main()
{
    // Places the glyphs of a 4bpp boxed dinooid sprite
    //   into LOW obj memory (cbb == 4)
    memcpy32(&tile_mem[4][0], dinoTiles, dinoTilesLen / sizeof(u32));
    memcpy16(pal_obj_mem, dinoPal, dinoPalLen / sizeof(u16));

    oam_init(obj_buffer, 128);
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;


    int x= 96, y= 32;
    u32 tid= 0, pb= 0;        // tile id, pal-bank

    OBJ_ATTR *dino= &obj_buffer[0];
    obj_set_attr(dino,
      ATTR0_SQUARE, // Square, regular sprite
      ATTR1_SIZE_64, // 64x64p,
      ATTR2_PALBANK(pb) | tid);   // palbank 0, tile 0

    while(1) {
      vid_vsync();
      key_poll();
      
      // increment/decrement starting tile with R/L
      tid += bit_tribool(key_hit(-1), KI_R, KI_L);

      dino->attr2= ATTR2_BUILD(tid, pb, 0);
      obj_set_pos(dino, x, y);

      oam_copy(oam_mem, obj_buffer, 1);   // (6) Update OAM (only one now)
    }
    
    return 0;
}
