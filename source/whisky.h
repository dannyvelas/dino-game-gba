//
// whisky - a collection of stateless RNGs
// by Sean Connelly (@velipso), https://sean.fun
// Project Home: https://github.com/velipso/whisky
// SPDX-License-Identifier: 0BSD
//

#ifndef WHISKY__H
#define WHISKY__H

#include <stdint.h>

//
// HASH FUNCTIONS
//
// Note: the primary and alternates pass all dieharder tests and are of equal
// quality
//

static inline uint32_t whisky1(uint32_t i0) {
  uint32_t z0 = (i0 * 1831267127) ^ i0;
  uint32_t z1 = (z0 * 3915839201) ^ (z0 >> 20);
  uint32_t z2 = (z1 * 1561867961) ^ (z1 >> 24);
  return z2;
}

#endif
