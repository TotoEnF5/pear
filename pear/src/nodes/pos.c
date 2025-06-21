#include "nodes/pos.h"
#include "node.h"

NODE_NEW_FUNC_SIGNATURE(pos_t, f32 x, f32 y, f32 z) {
    pos_t* self = node_new(pos_t, parent, NODE_TYPE_POS, name);
    self->pos[0] = x;
    self->pos[1] = y;
    self->pos[2] = z;
    return self;
}
