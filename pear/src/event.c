#include "event.h"
#include "array.h"

static event_func_array_t* funcs;
static ptr_array_t* user_datas;

void event_init() {
    funcs = event_func_array_new();
    user_datas = ptr_array_new();
}

void event_terminate() {
    event_func_array_delete(funcs);
    ptr_array_delete(user_datas);
}

void event_send(event_type_t type, void* event) {
    for (u32 i = 0; i < funcs->length; i++) {
        event_func_t func = event_func_array_get(funcs, i);
        void* user_data = ptr_array_get(user_datas, i);

        func(type, event, user_data);
    }
}

void event_subscribe(event_func_t func, void* user_data) {
    event_func_array_add(funcs, func);
    ptr_array_add(user_datas, user_data);
}
