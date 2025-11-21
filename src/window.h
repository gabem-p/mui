#pragma once

#include <GLFW/glfw3.h>
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include <mstd/types/list.h>

typedef struct {
    GLFWwindow* glfwWindow;
    void (*loop)(void*);
    void* layout;
} mui_window;

typedef void (*mui_loop_function)(mui_window*);

extern list* windows;
extern mui_window* activeWindow;

mui_window* mui_window_new(vec2 size, string title);
void mui_window_destroy(mui_window* window);
void mui_window_set_loop(mui_window* window, mui_loop_function function);