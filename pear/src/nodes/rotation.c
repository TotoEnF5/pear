#include "nodes/rotation.h"

NODE_NEW_FUNC_SIGNATURE(rotation_t, f32 x, f32 y, f32 z) {
    rotation_t* self = node_new(rotation_t, parent, NODE_TYPE_ROTATION, name);
    self->rotation[0] = x;
    self->rotation[1] = y;
    self->rotation[2] = z;
    return self;
}
