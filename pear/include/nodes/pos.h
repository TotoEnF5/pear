#ifndef NODE_POS_H_
#define NODE_POS_H_

#include "node.h"

NODE_TYPE_DECL(
    pos_t, 
    vec3 pos;
);

NODE_NEW_FUNC_SIGNATURE(pos_t, f32 x, f32 y, f32 z);

#endif
