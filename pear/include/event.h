#ifndef EVENT_H_
#define EVENT_H_

#include "array.h"

typedef enum event_type_t {
    EVENT_TYPE_QUIT,
    EVENT_TYPE_WINDOW_RESIZED,
} event_type_t;

typedef struct window_resized_event_t {
    u32 width;
    u32 height;
} window_resized_event_t;

typedef void(*event_func_t)(event_type_t type, void* event, void* user_data);
ARRAY_DECL(event_func_t, event_func);

void event_init();
void event_terminate();
void event_send(event_type_t type, void* event);
void event_subscribe(event_func_t func, void* user_data);

#endif
