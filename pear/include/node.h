#ifndef NODE_H_
#define NODE_H_

#include <array.h>
#include <types.h>

typedef struct world_t world_t;
typedef struct node_t node_t;
ARRAY_DECL(node_t*, node);

typedef enum node_type_t {
    NODE_TYPE_CONTAINER,
    NODE_TYPE_TRANSFORM,
    NODE_TYPE_MESH,
    NODE_TYPE_SCRIPT,
} node_type_t;

typedef struct node_t {
    world_t* world;
    node_t* parent;
    node_array_t* children;
    node_type_t type;
    void* data;
} node_t;

node_t* node_new(world_t* world, node_t* parent, node_type_t type);
void node_delete(node_t* self);

void node_update(node_t* self);
node_t* node_new_child(node_t* self, node_type_t type);

#endif
