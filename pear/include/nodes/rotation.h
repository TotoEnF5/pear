#ifndef NODE_ROTATION_H_
#define NODE_ROTATION_H_

#include "node.h"

NODE_TYPE_DECL(
    rotation_t,
    vec3 rotation;
);

NODE_NEW_FUNC_SIGNATURE(rotation_t, f32 x, f32 y, f32 z);

#endif
