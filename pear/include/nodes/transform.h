#ifndef NODE_TRANSFORM_H_
#define NODE_TRANSFORM_H_

#include "node.h"

typedef struct transform_t {
    NODE_STUFF();

    vec3 pos;
    vec3 rotation;
    vec3 scale;
} transform_t;

NODE_INIT_FUNC_SIGNATURE(transform_t);
NODE_DELETE_FUNC_SIGNATURE(transform_t);

#endif
