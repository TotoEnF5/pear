#ifndef ALLOC_H_
#define ALLOC_H_

#include <types.h>

void* pear_malloc(size_t size);
void* pear_realloc(void* ptr, size_t size);
void pear_free(void* ptr);

#define PEAR_ALLOC(type) (type*)pear_malloc(sizeof(type))
#define PEAR_ALLOC_N(type, n) (type*)pear_malloc(sizeof(type) * (n))

#endif
