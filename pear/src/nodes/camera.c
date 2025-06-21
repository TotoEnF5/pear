#include "nodes/camera.h"
#include "node.h"

NODE_NEW_FUNC_SIGNATURE(camera_t) {
    camera_t* self = node_new(camera_t, parent, NODE_TYPE_CAMERA, name);
    self->use = true;
    return self;
}
