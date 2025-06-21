#include "app.h"
#include "renderer.h"
#include "window.h"

#define SOKOL_TIME_IMPL
#include "sokol_time.h"

static app_t app = { 0 };

void app_init() {
    app.window = window_new("pear app", 800, 600); 
    app.renderer = renderer_new();
    app.is_running = false;

    stm_setup();
    app.last_time = stm_now();
}

void app_delete() {
    node_delete(app.root);
    renderer_delete(app.renderer);
    window_delete(app.window);
}

void app_set_root(node_t *root) {
    app.root = root;
}

void app_run() {
    app.is_running = true;
    while (app.is_running) {
        app.dt = stm_sec(stm_laptime(&app.last_time));

        window_update(app.window);
        renderer_render_node(app.renderer, app.root);
        window_swap_buffers(app.window);

        node_update(app.root, app.dt);

        app.is_running = !window_should_close(app.window);
    }

    app_delete();
}

app_t* app_get() {
    return &app;
}
