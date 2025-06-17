#ifndef ARENA_H_
#define ARENA_H_

#include <stddef.h>

typedef struct arena_t {
    void* buffer;
    size_t ptr;
    size_t capacity;
} arena_t;

arena_t* arena_new(size_t capacity);
void arena_delete(arena_t* self);

void* arena_alloc(arena_t* self, size_t size);

#endif
