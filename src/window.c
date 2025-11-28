#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#include <string.h>
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include <mstd/types/list.h>
#include "src/render/draw.h"

typedef bool (*mui_layout_function)(void*);

typedef struct {
    GLFWwindow* glfwWindow;
    mui_layout_function layoutFunction;
    void* layout;
    uint vao;
} mui_window;

list* windows;
mui_window* activeWindow;

mui_window* mui_window_new(vec2 size, string title) {
    GLFWwindow* root = null;
    if (windows->length == 0)
        glfwWindowHint(GLFW_VISIBLE, false);
    else {
        root = ((mui_window*)list_get(windows, 0))->glfwWindow;
        glfwWindowHint(GLFW_VISIBLE, true);
    }

    GLFWwindow* glfwWindow = glfwCreateWindow(size.x, size.y, title, null, root);

    glfwMakeContextCurrent(glfwWindow);

    mui_window* window = malloc(sizeof(mui_window));
    memset(window, 0, sizeof(mui_window));
    window->glfwWindow = glfwWindow;

    glClearColor(1, 1, 1, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glGenVertexArrays(1, &window->vao);

    list_add(windows, window);
    return window;
}

void mui_window_destroy(mui_window* window) {
    glfwDestroyWindow(window->glfwWindow);
    list_remove(windows, list_index(windows, window));
    // free(window->layout);
    free(window);
}

void layout_init(mui_window* window);
void mui_window_layout(mui_window* window, mui_layout_function function) {
    window->layoutFunction = function;
    layout_init(window);

    activeWindow = window;
    mui_defaults();
    function(window);
    mui_layout_compute(window->layout);
}