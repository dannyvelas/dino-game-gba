#ifndef OBJ_BUFFER_H
#define OBJ_BUFFER_H

#include <tonc.h>

#define OBJ_BUFFER_CAP 128

struct buffer_state {
  int len;
  OBJ_ATTR obj_buffer[OBJ_BUFFER_CAP];
};

struct buffer_state init_buffer();

OBJ_ATTR *alloc_obj(struct buffer_state *buffer_state);

#endif // OBJ_BUFFER_H
