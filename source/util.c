#include "util.h"

int tiles_to_pixels(int amt_tiles) { return amt_tiles * 8; }
int pixels_to_tiles(int amt_pixels) { return amt_pixels / 8; }
