#include <string.h>
#include <tonc.h>
#include "dino.h"
#include "sky.h"

// convention is to make the screen base block as far right in VRAM as you can
// we are using a 32x32t map so we can use the very last screen block
#define SBB_index 31
#define CBB_index 0

int main() {
    // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
    REG_DISPCNT= DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

    // set the background using charblock 0 as the character base block
    // and SBB. 4bpp background tiles and 32x32 tiles
    REG_BG0CNT= BG_CBB(CBB_index) | BG_SBB(SBB_index) | BG_4BPP | BG_REG_32x32;
    // set scrolling registers to 0
    REG_BG0HOFS= 0;
    REG_BG0VOFS= 0;

    // load background data into memory
    memcpy32(pal_bg_mem, skyPal, skyPalLen / sizeof(u32));
    memcpy16(&tile_mem[CBB_index][0], skyTiles, skyTilesLen / sizeof(u16)); 

    // load background tile maps
    SCR_ENTRY *bg0_tilemap= se_mem[SBB_0]; 
    
    // place dino sprite
    memcpy32(&tile_mem[4][0], dinoTiles, dinoTilesLen / sizeof(u32));
    memcpy16(pal_obj_mem, dinoPal, dinoPalLen / sizeof(u16));

    // init OAM for this dino
    OBJ_ATTR *dino = &oam_mem[0];
    oam_init(dino, 1);

    int x= 12, y= 96;
    u32 tid= 0, pb= 0;

    obj_set_attr(dino,
      ATTR0_SQUARE,
      ATTR1_SIZE_32,
      ATTR2_PALBANK(pb) | tid);
    obj_set_pos(dino, x, y);

    while(1) {
        vid_vsync();
        key_poll();
 
        pb= key_is_down(KEY_SELECT) ? 1 : 0;
        dino->attr2= ATTR2_BUILD(tid, pb, 0);
    }
    
    return 0;
}
