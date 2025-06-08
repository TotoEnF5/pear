#include "window.h"
#include "alloc.h"
#include "log.h"
#include "GLFW/glfw3.h"

typedef struct window_t {
    GLFWwindow* handle;
} window_t;

window_t* window_new(const char* title, u32 width, u32 height) {
    PEAR_ASSERT(glfwInit() == GLFW_TRUE, "failed to initialize glfw!");

    window_t* self = PEAR_ALLOC(window_t);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    self->handle = glfwCreateWindow((int)width, (int)height, title, NULL, NULL);
    PEAR_ASSERT(self->handle != NULL, "failed to create window!");

    glfwMakeContextCurrent(self->handle);

    return self;
}

void window_delete(window_t* self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();

    pear_free(self);
}

bool window_should_close(window_t *self) {
    return glfwWindowShouldClose(self->handle);
}

void window_update(window_t *self) {
    glfwPollEvents();
}

void window_swap_buffers(window_t *self) {
    glfwSwapBuffers(self->handle);
}
