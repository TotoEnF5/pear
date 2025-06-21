#ifndef ARRAY_H_
#define ARRAY_H_

#include "types.h"
#include "alloc.h"
#include "log.h"
#include <string.h>

#define ARRAY_DECL_NAMED(type, array_type, prefix) \
    typedef struct array_type { \
        type* buffer; \
        u32 length; \
        u32 capacity; \
    } array_type;\
\
    inline static array_type* prefix##_new(void) { \
        array_type* self = PEAR_ALLOC(array_type); \
        self->capacity = 3; \
        self->buffer = PEAR_ALLOC_N(type, self->capacity); \
        self->length = 0; \
        return self; \
    } \
\
    inline static void prefix##_delete(array_type* self) { \
        pear_free(self->buffer); \
        pear_free(self); \
    } \
\
    inline static void prefix##_reserve(array_type* self, size_t size) { \
        if (size > self->capacity) { \
            self->buffer = (type*)pear_realloc(self->buffer, size); \
            self->capacity = size; \
        } \
    } \
\
    inline static void prefix##_add(array_type* self, type elem) { \
        if (self->capacity <= self->length) { \
            self->capacity += self->capacity / 2 + 1; \
            self->buffer = (type*)pear_realloc(self->buffer, self->capacity * sizeof(type)); \
        } \
        self->buffer[self->length] = elem; \
        self->length++; \
    } \
\
    inline static void prefix##_insert(array_type* self, type elem, u32 index) { \
        PEAR_ASSERT(index >= 0 && index < self->length, "out of bounds index insert!"); \
        if (self->capacity <= self->length) { \
            self->capacity += self->capacity / 2 + 1; \
            self->buffer = (type*)pear_realloc(self->buffer, self->capacity * sizeof(type)); \
        } \
        memmove(&self->buffer[index + 1], &self->buffer[index], self->length - index); \
        self->buffer[index] = elem; \
        self->length++; \
    } \
\
    inline static type prefix##_get(array_type* self, u32 index) { \
        PEAR_ASSERT(index >= 0 && index < self->length, "out of bounds index access!"); \
        return self->buffer[index]; \
    } \
\
    inline static void prefix##_remove_index(array_type* self, u32 index) { \
        PEAR_ASSERT(index >= 0 && index < self->length, "out of bounds index removal!"); \
        memmove(&self->buffer[index], &self->buffer[index + 1], self->length - index); \
        self->length--; \
    } \
\
    inline static i32 prefix##_get_index(array_type* self, type element) { \
        for (i32 i = 0; i < self->length; i++) { \
            if (memcmp(&element, &(self)->buffer[i], sizeof(type)) == 0) { \
                return i; \
            } \
        } \
        return -1; \
    } \
\
    inline static void prefix##_remove(array_type* self, type element) { \
        prefix##_remove_index(self, prefix##_get_index(self, element)); \
    }

#define ARRAY_DECL(type, prefix) ARRAY_DECL_NAMED(type, prefix##_array_t, prefix##_array)

#define ARRAY_FOREACH(self, elem, i) for ((i) = 0, (elem) = (self)->buffer[0]; (i) < (self)->length; (i)++, (elem) = (self)->buffer[i])

ARRAY_DECL(u32, u32);
ARRAY_DECL(f32, f32);
ARRAY_DECL(void*, ptr);

#endif
