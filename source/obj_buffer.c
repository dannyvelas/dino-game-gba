#include "obj_buffer.h"

struct buffer_state init_buffer() {
  struct buffer_state buffer_state = {
      .len = 0,
      .obj_buffer = {},
  };
  oam_init(buffer_state.obj_buffer, OBJ_BUFFER_CAP);
  return buffer_state;
}

OBJ_ATTR *alloc_obj(struct buffer_state *buffer_state) {
  int open_slot = buffer_state->len;
  buffer_state->len += 1;
  return &buffer_state->obj_buffer[open_slot];
}
