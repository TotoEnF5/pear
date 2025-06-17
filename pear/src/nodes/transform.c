#include "nodes/transform.h"
#include "node.h"

NODE_INIT_FUNC_SIGNATURE(transform_t) {
    self->type = NODE_TYPE_TRANSFORM;
    glm_vec3_zero(self->pos);
    glm_vec3_zero(self->rotation);
    glm_vec3_one(self->scale);
    return self;
}

NODE_DELETE_FUNC_SIGNATURE(transform_t) {
    NODE_FREE(self);
}
