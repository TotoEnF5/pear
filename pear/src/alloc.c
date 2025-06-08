#include "alloc.h"
#include "log.h"
#include <stdlib.h>

void* pear_malloc(size_t size) {
    void* ptr = malloc(size);
    PEAR_ASSERT(ptr != NULL, "failed to allocate %d bytes!", size);
    return ptr;
}

void* pear_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    PEAR_ASSERT(new_ptr != NULL, "failed to realloc %p for %d bytes!", ptr, size);
    return new_ptr;
}

void pear_free(void* ptr) {
    free(ptr);
}
