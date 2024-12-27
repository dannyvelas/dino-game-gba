#include <string.h>
#include <tonc.h>
#include "character.h"

int main() {
    OBJ_ATTR *character = &oam_mem[0];
    
    // Place the glyphs of a 4bpp boxed character sprite
    memcpy32(&tile_mem[4][0], characterTiles, characterTilesLen / sizeof(u32));
    memcpy16(pal_obj_mem, characterPal, characterPalLen / sizeof(u16));

    // init OAM for this character
    oam_init(character, 1);
    REG_DISPCNT= DCNT_OBJ | DCNT_OBJ_1D;

    int x= 96, y= 32;
    u32 tid= 0, pb= 0;        // tile id, pal-bank

    obj_set_attr(character,
      ATTR0_SQUARE, // Square, regular sprite
      ATTR1_SIZE_32, // 32x32p
      ATTR2_PALBANK(pb) | tid);   // palbank 0, tile 0
    obj_set_pos(character, x, y);

    while(1) {
        vid_vsync();
        key_poll();
 
        if (key_hit(KEY_L)) {
          tid -= 32;
        } else if (key_hit(KEY_R)) {
          tid += 32;
        }
        
        character->attr2= ATTR2_BUILD(tid, pb, 0);
    }
    
    return 0;
}
