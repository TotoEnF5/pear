#ifndef NODE_CAMERA_H_
#define NODE_CAMERA_H_

#include "node.h"

NODE_TYPE_DECL(
    camera_t,
    bool use;
);

NODE_NEW_FUNC_SIGNATURE(camera_t);

#endif
