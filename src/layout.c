#include "src/window.h"
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include <mstd/types/list.h>
#include <mstd/types/stack.h>

enum element_type {
    ELEMENT_TEXT,
    ELEMENT_REGION
};

typedef void (*mui_element_callback)(void*);

typedef struct {
    enum element_type type;
    void* layout;
    void* parent;

    union {
        struct {
            string text;
            float size;
        } text;
        struct {
            list* children;
        } region;
    };
} mui_element;

typedef struct {
    list* elements;
    mui_window* window;

    stack* regionStack;
} mui_layout;

typedef struct {
    uint textSize;
} layout_settings;
layout_settings settings;

void layout_init(mui_window* window) {
    mui_layout* layout = malloc(sizeof(mui_layout));
    layout->elements = list_new();
    layout->window = window;
    layout->regionStack = stack_new(64);

    window->layout = layout;
}

mui_element* element_new(enum element_type type) {
    mui_element* element = malloc(sizeof(mui_element));
    mui_layout* layout = activeWindow->layout;

    element->type = type;
    element->layout = layout;
    element->parent = stack_peek(layout->regionStack);

    return element;
}

void mui_defaults() {
    settings.textSize = 15;
}

void mui_text(string text) {
    mui_element* element = element_new(ELEMENT_TEXT);
    element->text.text = text;
    element->text.size = settings.textSize;
}