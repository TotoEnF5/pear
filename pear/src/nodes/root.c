#include "nodes/root.h"
#include "alloc.h"
#include "node.h"
#include <string.h>

root_t* root_new(const char* name) {
    root_t* self = PEAR_ALLOC(root_t);
    self->parent = NULL;
    self->children = node_array_new();
    strncpy(self->name, name, NODE_NAME_LENGTH);
    return self;
}

void root_delete(root_t *self) {
    NODE_FREE(self);
}
