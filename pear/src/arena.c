#include "arena.h"
#include "alloc.h"
#include "log.h"

arena_t* arena_new(size_t capacity) {
    arena_t* self = PEAR_ALLOC(arena_t);
    self->capacity = capacity;
    self->ptr = 0;
    self->buffer = pear_malloc(capacity);
    return self;
}

void arena_delete(arena_t* self) {
    pear_free(self->buffer);
    pear_free(self);
}

void* arena_alloc(arena_t* self, size_t size) {
    if (self->ptr + size >= self->capacity) {
        PEAR_WARN("arena size exceeded!");
        return NULL;
    }

    void* ptr = self->buffer + self->ptr;
    self->ptr += size;
    return ptr;
}
