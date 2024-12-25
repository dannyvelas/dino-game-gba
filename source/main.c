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

    // position sprite (redundant here; the _real_ position
    // is set further down
    obj_set_pos(dino, x, y);

    while(1)
    {
        vid_vsync();
        key_poll();

        // (5) Do various interesting things
        // move left/right
        x += 2*key_tri_horz();

        // move up/down
        y += 2*key_tri_vert();

        // increment/decrement starting tile with R/L
        tid += bit_tribool(key_hit(-1), KI_R, KI_L);

        // flip
        if(key_hit(KEY_A))  // horizontally
            dino->attr1 ^= ATTR1_HFLIP;
        if(key_hit(KEY_B))  // vertically
            dino->attr1 ^= ATTR1_VFLIP;

        // make it glow (via palette swapping)
        pb= key_is_down(KEY_SELECT) ? 1 : 0;

        // toggle mapping mode
        if(key_hit(KEY_START))
            REG_DISPCNT ^= DCNT_OBJ_1D;

        // Hey look, it's one of them build macros!
        dino->attr2= ATTR2_BUILD(tid, pb, 0);
        obj_set_pos(dino, x, y);

        oam_copy(oam_mem, obj_buffer, 1);   // (6) Update OAM (only one now)
    }
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
