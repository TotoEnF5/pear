#include "nodes/scale.h"

NODE_NEW_FUNC_SIGNATURE(scale_t, f32 x, f32 y, f32 z) {
    scale_t* self = node_new(scale_t, parent, NODE_TYPE_SCALE, name);
    self->scale[0] = x;
    self->scale[1] = y;
    self->scale[2] = z;
    return self;
}
