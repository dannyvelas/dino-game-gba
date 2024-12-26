#include <string.h>
#include <tonc.h>
#include "dino.h"

OBJ_ATTR obj_buffer[128];
OBJ_AFFINE *obj_aff_buffer= (OBJ_AFFINE*)obj_buffer;

void obj_test()
{
    int x= 96, y= 32;
    u32 tid= 0, pb= 0;        // tile id, pal-bank

    OBJ_ATTR *dino= &obj_buffer[0];
    obj_set_attr(dino,
      ATTR0_SQUARE, // Square, regular sprite
      ATTR1_SIZE_64, // 64x64p,
      ATTR2_PALBANK(pb) | tid);   // palbank 0, tile 0

      vid_vsync();

      // Hey look, it's one of them build macros!
      dino->attr2= ATTR2_BUILD(tid, pb, 0);
      obj_set_pos(dino, x, y);

      oam_copy(oam_mem, obj_buffer, 1);   // (6) Update OAM (only one now)
}

int main()
{
    // Places the glyphs of a 4bpp boxed dinooid sprite
    //   into LOW obj memory (cbb == 4)
    memcpy32(&tile_mem[4][0], dinoTiles, dinoTilesLen / sizeof(u32));
    memcpy16(pal_obj_mem, dinoPal, dinoPalLen / sizeof(u16));

    oam_init(obj_buffer, 128);
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;

    obj_test();

    while(1);

    return 0;
}
