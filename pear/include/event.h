#ifndef EVENT_H_
#define EVENT_H_

#include "array.h"

typedef enum event_type_t {
    EVENT_TYPE_QUIT,
    EVENT_TYPE_WINDOW_RESIZED,
    EVENT_TYPE_KEY_PRESSED,
    EVENT_TYPE_KEY_RELEASED,
    EVENT_TYPE_BUTTON_PRESSED,
    EVENT_TYPE_BUTTON_RELEASED,
    EVENT_TYPE_MOUSE_MOVED,
} event_type_t;

typedef struct window_resized_event_t {
    u32 width;
    u32 height;
} window_resized_event_t;

typedef struct key_event_t {
    i32 key;
} key_event_t;

typedef struct button_event_t {
    i32 button;
} button_event_t;

typedef struct mouse_moved_event_t {
    f64 x;
    f64 y;
    f64 rel_x;
    f64 rel_y;
} mouse_moved_event_t;

typedef void(*event_func_t)(event_type_t type, void* event, void* user_data);
ARRAY_DECL(event_func_t, event_func);

void event_init();
void event_terminate();
void event_send(event_type_t type, void* event);
void event_subscribe(event_func_t func, void* user_data);

#endif
