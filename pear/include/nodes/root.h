#ifndef NODE_ROOT_H_
#define NODE_ROOT_H_

#include "node.h"

typedef struct root_t {
    NODE_STUFF();
} root_t;

root_t* root_new(const char* name);
void root_delete(root_t* self);

#endif
