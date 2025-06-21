#include "window.h"
#include "event.h"
#include "alloc.h"
#include "log.h"
#include "GLFW/glfw3.h"

typedef struct window_t {
    GLFWwindow* handle;
    f64 last_mouse_x;
    f64 last_mouse_y;
} window_t;

static void window_close_callback(GLFWwindow* window) {
    event_send(EVENT_TYPE_QUIT, NULL);
}

static void window_resize_callback(GLFWwindow* window, int width, int height) {
    event_send(EVENT_TYPE_WINDOW_RESIZED, &(window_resized_event_t){ .width = (u32)width, .height = (u32)height });
}

static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    event_type_t type = action == GLFW_PRESS ? EVENT_TYPE_KEY_PRESSED : EVENT_TYPE_KEY_RELEASED;
    event_send(type, &(key_event_t){ .key = key });
}

static void window_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    event_type_t type = action == GLFW_PRESS ? EVENT_TYPE_BUTTON_PRESSED : EVENT_TYPE_BUTTON_RELEASED;
    event_send(type, &(button_event_t){ .button = button });
}

static void window_mouse_moved_callback(GLFWwindow* window, double x, double y) {
    window_t* self = (window_t*)glfwGetWindowUserPointer(window);
    event_send(EVENT_TYPE_MOUSE_MOVED, &(mouse_moved_event_t){
        .x = x,
        .y = y,
        .rel_x = x - self->last_mouse_x,
        .rel_y = y - self->last_mouse_y
    });
    self->last_mouse_x = x;
    self->last_mouse_y = y;
}

window_t* window_new(const char* title, u32 width, u32 height) {
    PEAR_ASSERT(glfwInit() == GLFW_TRUE, "failed to initialize glfw!");

    window_t* self = PEAR_ALLOC(window_t);
    self->last_mouse_x = 0.0f;
    self->last_mouse_y = 0.0f;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    self->handle = glfwCreateWindow((int)width, (int)height, title, NULL, NULL);
    PEAR_ASSERT(self->handle != NULL, "failed to create window!");

    glfwSetWindowUserPointer(self->handle, self);

    glfwMakeContextCurrent(self->handle);

    glfwSetWindowCloseCallback(self->handle, window_close_callback);
    glfwSetWindowSizeCallback(self->handle, window_resize_callback);
    glfwSetKeyCallback(self->handle, window_key_callback);
    glfwSetMouseButtonCallback(self->handle, window_mouse_button_callback);
    glfwSetCursorPosCallback(self->handle, window_mouse_moved_callback);

    return self;
}

void window_delete(window_t* self) {
    glfwDestroyWindow(self->handle);
    glfwTerminate();

    pear_free(self);
}

void window_update(window_t *self) {
    glfwPollEvents();
}

void window_swap_buffers(window_t *self) {
    glfwSwapBuffers(self->handle);
}
