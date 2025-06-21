#ifndef NODE_H_
#define NODE_H_

#include "array.h"

/**
 * to declare a new node type:
 *
 * 1. add the node type to the node_type_t enum
 *
 * 2. create a header file in nodes/ wit hthe following contents:
 *
 * NODE_TYPE_DECL(
 *     node_type_name_t,
 *     int foo,
 *     float bar,
 *     ...
 * );
 *
 * NODE_NEW_FUNC_SIGNATURE(node_type_name_t, ...);
 *
 * 3. create a source file in src/nodes/ with the following contents:
 *
 * NODE_NEW_FUNC_SIGNATURE(node_type_name_t, ...) {
 *     node_type_name_t* self = node_new(parent, node_type_name_t, name, ...);
 *     // do stuff
 *     return self;
 * }
 */

typedef enum node_type_t {
    NODE_TYPE_ROOT,
    NODE_TYPE_CONTAINER,
    NODE_TYPE_POS,
    NODE_TYPE_ROTATION,
    NODE_TYPE_SCALE,
    NODE_TYPE_MODEL,
    NODE_TYPE_MESH,
    NODE_TYPE_SCRIPT,
} node_type_t;

typedef struct node_t node_t;
ARRAY_DECL(node_t*, node);

#define NODE_NAME_LENGTH 128
#define NODE_STUFF() \
    node_type_t type; \
    node_t* parent; \
    node_array_t* children; \
    char name[NODE_NAME_LENGTH]; \
    void(*delete_func)(node_t* self)

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

void node_update(node_t* self, f32 dt);
node_t* node_get_sibling_by_name(node_t* self, const char* name);

node_t* _node_new(size_t size, node_t* parent, node_type_t type, const char* name);

#endif
