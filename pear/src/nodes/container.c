#include "nodes/container.h"

NODE_NEW_FUNC_SIGNATURE(container_t) {
    container_t* self = node_new(container_t, parent, NODE_TYPE_CONTAINER, name);
    return self;
}
