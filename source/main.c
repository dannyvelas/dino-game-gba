#include <string.h>
#include <tonc.h>
#include "dino.h"
#include "sky.h"

// convention is to make the screen base block as far right in VRAM as you can
// we are using a 32x32t map so we can use the very last screen block
#define SBB_INDEX 31
#define CBB_INDEX 0

#define TILE_HEIGHT 8
#define AMT_ROWS (SCREEN_HEIGHT / TILE_HEIGHT)

int main() {
    // set I/O register to use mode0, sprites, 1d sprites and tiled background 0
    REG_DISPCNT= DCNT_MODE0 | DCNT_OBJ | DCNT_OBJ_1D | DCNT_BG0;

    // set the background using charblock 0 as the character base block
    // and SBB. 4bpp background tiles and 32x32 tiles
    REG_BG0CNT= BG_CBB(CBB_INDEX) | BG_SBB(SBB_INDEX) | BG_4BPP | BG_REG_32x32;
    // set scrolling registers to 0
    REG_BG0HOFS= 0;
    REG_BG0VOFS= 0;

    // load background data into memory
    memcpy32(pal_bg_mem, skyPal, skyPalLen / sizeof(u32));
    memcpy16(&tile_mem[CBB_INDEX][0], skyTiles, skyTilesLen / sizeof(u16)); 

    // load background tile maps
    int floor_tile_y = (AMT_ROWS-4); // lets make the floor 3 tiles up from bottom
    int floor_tile_index = 1;
    toncset16(&se_mem[SBB_INDEX][floor_tile_y*32], floor_tile_index, 32);
    
    // place dino sprite into very first sprite charblock (4)
    memcpy32(&tile_mem[4][0], dinoTiles, dinoTilesLen / sizeof(u32));
    memcpy16(pal_obj_mem, dinoPal, dinoPalLen / sizeof(u16));

    // init OAM for this dino
    OBJ_ATTR *dino = &oam_mem[0];
    oam_init(dino, 1);

    // initialize dino attributes; also lets make our dinosaur be 4 tiles above the floor
    int x = 12;
    int y = (floor_tile_y-4)*TILE_HEIGHT; 
    u32 tile_index= 0, palette_bank= 0;
    obj_set_attr(dino, ATTR0_SQUARE, ATTR1_SIZE_32, ATTR2_PALBANK(palette_bank) | tile_index);

    // set initial position of dino
    obj_set_pos(dino, x, y);

    while(1) {
        vid_vsync();
        key_poll();
 
        palette_bank= key_is_down(KEY_SELECT) ? 1 : 0;
        dino->attr2= ATTR2_BUILD(tile_index, palette_bank, 0);
    }
    
    return 0;
}
