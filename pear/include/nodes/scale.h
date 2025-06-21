#ifndef NODE_SCALE_H_
#define NODE_SCALE_H_

#include "node.h"

NODE_TYPE_DECL(
    scale_t,
    vec3 scale;
);

NODE_NEW_FUNC_SIGNATURE(scale_t, f32 x, f32 y, f32 z);

#endif
