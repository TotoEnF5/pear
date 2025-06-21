#ifndef NODE_H_
#define NODE_H_

#include "array.h"

/**
 * to declare a new node type:
 *
 * NODE_TYPE_DECL(
 *     node_type_name_t,
 *     int foo,
 *     float bar,
 *     ...
 * );
 *
 * NODE_NEW_FUNC_SIGNATURE(node_type_name_t, ...);
 */

typedef enum node_type_t {
    NODE_TYPE_ROOT,
    NODE_TYPE_CONTAINER,
    NODE_TYPE_POS,
    NODE_TYPE_ROTATION,
    NODE_TYPE_SCALE,
    NODE_TYPE_MODEL,
    NODE_TYPE_MESH,
} node_type_t;

typedef struct node_t node_t;
ARRAY_DECL(node_t*, node);

#define NODE_NAME_LENGTH 128
#define NODE_STUFF() \
    node_type_t type; \
    node_t* parent; \
    node_array_t* children; \
    char name[NODE_NAME_LENGTH]

typedef struct node_t {
    NODE_STUFF();
} node_t;

#define NODE_TYPE_DECL(typename, attributes) typedef struct typename { NODE_STUFF(); attributes } typename
#define NODE_NEW_FUNC_NAME(typename) typename##_new
#define NODE_NEW_FUNC_SIGNATURE(typename,  ...) typename* NODE_NEW_FUNC_NAME(typename)(node_t* parent, const char* name, ##__VA_ARGS__)

#define node_new(typename, parent, type, name) (typename*)_node_new(sizeof(typename), (parent), (type), (name))
#define node_child(self, typename, name, ...) NODE_NEW_FUNC_NAME(typename)((node_t*)(self), (name), ##__VA_ARGS__)
node_t* node_root(const char* name);
void node_delete(node_t* self);

node_t* _node_new(size_t size, node_t* parent, node_type_t type, const char* name);

#endif
