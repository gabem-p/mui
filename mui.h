#pragma once

#include <mstd/common.h>
#include <mstd/types/primitives.h>

bool mui_init();
void mui_destroy();
void mui_exit();
void mui_loop();

typedef struct {
} mui_window;

typedef void (*mui_layout_function)(mui_window*);

mui_window* mui_window_new(vec2 size, string title);
void mui_window_destroy(mui_window* window);
void mui_window_layout(mui_window* window, mui_layout_function function);

void mui_defaults();
void mui_text(string text);
void mui_text_size(uint size);

void mui_region_push();
void mui_region_pop();
#define mui_region mui_region_push(); for(uint i = 0; i++ == 0; mui_region_pop())