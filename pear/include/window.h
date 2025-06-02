#ifndef WINDOW_H_
#define WINDOW_H_

#include <types.h>

typedef struct window_t window_t;

window_t* window_new(const char* title, u32 width, u32 height);
void window_delete(window_t* self);

bool window_should_close(window_t* self);

void window_update(window_t* self);
void window_swap_buffers(window_t* self);

#endif
