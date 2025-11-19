#pragma once

#include <mstd/common.h>
#include <mstd/types/primitives.h>

bool mui_init();
void mui_destroy();
void mui_exit();
void mui_loop();

typedef struct {
} mui_window;

typedef void (*mui_loop_function)(mui_window*);

mui_window* mui_window_new(vec2 size, string title);
void mui_window_destroy(mui_window* window);
void mui_window_set_loop(mui_window* window, mui_loop_function function);

void mui_debug_draw_atlas();