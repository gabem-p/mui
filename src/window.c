#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string.h>
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include <mstd/types/list.h>
#include "src/render/draw.h"

typedef bool (*mui_loop_function)(void*);

typedef struct {
    GLFWwindow* glfwWindow;
    mui_loop_function loop;
    void* layout;
} mui_window;

list* windows;
mui_window* activeWindow;

mui_window* mui_window_new(vec2 size, string title) {
    GLFWwindow* glfwWindow = glfwCreateWindow(size.x, size.y, title, null, list_get(windows, 0));

    if (windows->length == 0)
        glfwMakeContextCurrent(glfwWindow);

    mui_window* window = malloc(sizeof(mui_window));
    memset(window, 0, sizeof(mui_window));
    window->glfwWindow = glfwWindow;

    list_add(windows, window);
    return window;
}

void mui_window_destroy(mui_window* window) {
    glfwDestroyWindow(window->glfwWindow);
    list_remove(windows, list_index(windows, window));
    free(window);
}

void mui_window_set_loop(mui_window* window, mui_loop_function function) {
    window->loop = function;
}