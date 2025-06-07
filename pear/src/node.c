#include <node.h>
#include <alloc.h>
#include <log.h>

void node_allocate_data(node_t* self) {
    switch (self->type) {
    default:
        PEAR_WARN("unknown node type %d!", self->type);
        break;
    }
}

node_t* node_new(world_t* world, node_t* parent, node_type_t type) {
    node_t* self = PEAR_ALLOC(node_t);

    self->world = world;
    self->parent = parent;
    self->children = node_array_new();
    self->type = type;

    node_allocate_data(self);

    return self;
}

void node_delete(node_t* self) {
    u32 i;
    node_t* child;
    ARRAY_FOREACH(self->children, child, i) {
        node_delete(child);
    }

    node_array_delete(self->children);
    pear_free(self);
}

void node_update(node_t* self) {
    switch (self->type) {
        default:
            break;
    }

    node_t* child;
    u32 i;
    ARRAY_FOREACH(self->children, child, i) {
        node_update(child);
    }
}

node_t* node_new_child(node_t* self, node_type_t type) {
    return node_new(self->world, self, type);
}
