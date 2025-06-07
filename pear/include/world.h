#ifndef WORLD_H_
#define WORLD_H_

#include <node.h>
#include <types.h>

typedef struct world_t {
    node_t* root;
} world_t;

world_t* world_new();
void world_delete(world_t* self);

#endif
