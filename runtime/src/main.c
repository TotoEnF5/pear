#include <pear.h>
#include <window.h>
#include <renderer.h>

int main(int argc, char* argv[]) {
    window_t* window = window_new("cool window", 800, 600);
    renderer_t* renderer = renderer_new();

    while (!window_should_close(window)) {
        window_update(window);
        renderer_render(renderer);
        window_swap_buffers(window);
    }

    renderer_delete(renderer);
    window_delete(window);

    return 0;
}
