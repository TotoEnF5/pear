#include <world.h>
#include <alloc.h>

world_t* world_new() {
    world_t* self = PEAR_ALLOC(world_t);
    self->root = &(node_t){
        .world = self,
        .children = node_array_new(),
        .parent = NULL,
        .type = NODE_TYPE_CONTAINER
    };
    return self;
}

void world_delete(world_t *self) {
    node_delete(self->root);
    pear_free(self);
}
