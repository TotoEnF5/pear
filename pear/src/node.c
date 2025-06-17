#include "node.h"
#include "alloc.h"
#include "array.h"
#include <string.h>

void* _node_child(node_t* self, const char* name, size_t size) {
    node_t* child = (node_t*)arena_alloc(self->arena, size);
    child->parent = self;
    child->children = node_array_new();
    strncpy(child->name, name, NODE_NAME_LENGTH);
    child->arena = self->arena;
    node_array_add(self->children, child);
    return child;
}

void node_map(node_t* self, void(*func)(node_t* node)) {
    func(self);

    node_t* child;
    u32 i;
    ARRAY_FOREACH(self->children, child, i) {
        node_map(child, func);
    }
}

void node_map_type(node_t* self, node_type_t type, void(*func)(node_t* node)) {
    if (self->type == type) {
        func(self);
    }

    node_t* child;
    u32 i;
    ARRAY_FOREACH(self->children, child, i) {
        node_map_type(child, type, func);
    }
}
