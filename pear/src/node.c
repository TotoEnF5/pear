#include "node.h"
#include "nodes/script.h"
#include "alloc.h"
#include "array.h"
#include <string.h>

node_t* _node_new(size_t size, node_t* parent, node_type_t type, const char* name) {
    node_t* self = pear_malloc(size);
    self->type = type;
    self->parent = parent;
    self->children = node_array_new();
    strncpy(self->name, name, NODE_NAME_LENGTH);
    self->delete_func = NULL;
    
    if (parent != NULL) {
        node_array_add(parent->children, self);
    }

    return self;
}

node_t* node_root(const char* name) {
    return _node_new(sizeof(node_t), NULL, NODE_TYPE_ROOT, name);
}

void node_delete(node_t* self) {
    if (self->delete_func != NULL) {
        self->delete_func(self);
    }

    node_t* child;
    u32 i;
    ARRAY_FOREACH(self->children, child, i) {
        node_delete(child);
    }

    node_array_delete(self->children);
    pear_free(self);
}

void node_update(node_t* self, f32 dt) {
    if (self->type == NODE_TYPE_SCRIPT) {
        if (((script_t*)self)->update != NULL) {
            ((script_t*)self)->update((script_t*)self, dt);
        }
    }

    node_t* child;
    u32 i;
    ARRAY_FOREACH(self->children, child, i) {
        node_update(child, dt);
    }
}

node_t* node_get_sibling_by_name(node_t* self, const char* name) {
    node_t* child;
    u32 i;
    ARRAY_FOREACH(self->parent->children, child, i) {
        if (strcmp(child->name, name) == 0) {
            return child;
        }
    }

    return NULL;
}
