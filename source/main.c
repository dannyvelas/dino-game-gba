#include <string.h>
#include <tonc.h>
#include "dino.h"
#include "sky.h"

int main() {
    OBJ_ATTR *dino = &oam_mem[0];

    // set the background
    memcpy32(pal_bg_mem, skyPal, skyPalLen / sizeof(u32));
    memcpy16(&tile_mem[0][0], skyTiles, skyTilesLen / sizeof(u16)); 
    
    // place dino sprite
    memcpy32(&tile_mem[4][0], dinoTiles, dinoTilesLen / sizeof(u32));
    memcpy16(pal_obj_mem, dinoPal, dinoPalLen / sizeof(u16));

    // init OAM for this dino
    oam_init(dino, 1);
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;

    int x= 12, y= 96;
    u32 tid= 0, pb= 0;        // tile id, pal-bank

    obj_set_attr(dino,
      ATTR0_SQUARE, // Square, regular sprite
      ATTR1_SIZE_32, // 32x32p
      ATTR2_PALBANK(pb) | tid);   // palbank 0, tile 0
    obj_set_pos(dino, x, y);

    while(1) {
        vid_vsync();
        key_poll();
 
        pb= key_is_down(KEY_SELECT) ? 1 : 0;
        dino->attr2= ATTR2_BUILD(tid, pb, 0);
    }
    
    return 0;
}
