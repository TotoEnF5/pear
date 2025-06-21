#ifndef APP_H_
#define APP_H_

#include "window.h"
#include "renderer.h"
#include "node.h"

typedef struct app_t {
    window_t* window;
    renderer_t* renderer;
    node_t* root;
    bool is_running;
} app_t;

void app_init();
void app_set_root(node_t* root);
void app_run();

app_t* app_get();

#endif
