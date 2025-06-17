#ifndef NODE_H_
#define NODE_H_

#include "arena.h"
#include "array.h"

#define NODE_NAME_LENGTH 128

typedef enum node_type_t {
    NODE_TYPE_ROOT,
    NODE_TYPE_TRANSFORM,
} node_type_t;

typedef struct node_t node_t;
ARRAY_DECL(node_t*, node);

#define NODE_STUFF() \
    node_t* parent; \
    node_array_t* children; \
    node_type_t type; \
    char name[NODE_NAME_LENGTH]; \
    arena_t* arena

typedef struct node_t {
    NODE_STUFF();
} node_t;

#define NODE_INIT_FUNC_NAME(type) type##_init
#define NODE_INIT_FUNC_SIGNATURE(type, ...) type* NODE_INIT_FUNC_NAME(type)(type* self, ##__VA_ARGS__)
#define NODE_DELETE_FUNC_NAME(type) type##_delete
#define NODE_DELETE_FUNC_SIGNATURE(type) void NODE_DELETE_FUNC_NAME(type)(type* self)

#define node_delete(self, type) NODE_DELETE_FUNC_NAME(type)((self))
#define node_child(self, type, name, ...) NODE_INIT_FUNC_NAME(type)(_node_child((node_t*)(self), (name), sizeof(type)), ##__VA_ARGS__)
void node_map(node_t* self, void(*func)(node_t* node));
void node_map_type(node_t* self, node_type_t type, void(*func)(node_t* node));

void* _node_child(node_t* self, const char* name, size_t size);

#define NODE_FREE(self) do { \
    if ((self)->parent != NULL) { \
        node_array_remove((self)->parent->children, (node_t*)(self)); \
    } \
    node_array_delete((self)->children); \
} while (0)

#endif
