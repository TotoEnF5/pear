#include "app.h"
#include "renderer.h"
#include "window.h"

static app_t app = { 0 };

void app_init() {
    app.window = window_new("pear app", 800, 600); 
    app.renderer = renderer_new();
    app.is_running = false;
}

void app_delete() {
    renderer_delete(app.renderer);
    window_delete(app.window);
}

void app_run() {
    app.is_running = true;
    while (app.is_running) {
        window_update(app.window);
        renderer_render(app.renderer);
        window_swap_buffers(app.window);

        app.is_running = !window_should_close(app.window);
    }

    app_delete();
}

app_t* app_get() {
    return &app;
}
