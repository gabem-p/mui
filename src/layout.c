#include <GLES3/gl32.h>
#include "src/window.h"
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include <mstd/types/list.h>
#include <mstd/types/stack.h>
#include "src/assets/text.h"

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
            uint id;
            uint count;
            list* children;
        } region;
    };

} mui_element;

typedef struct {
    list* elements;
    mui_window* window;

    list* regions;
    stack* regionStack;
} mui_layout;

typedef struct {
    uint textSize;
} layout_settings;
layout_settings settings;

void compute_elements(list* elements, vec2 cursor, vec2** buffer, uint* bufferSize) {
    list_iterator* iterator = list_iter_new(elements);
    for (uint i = 0; i < elements->length; i++, list_iter_next(iterator)) {
        mui_element* element = list_iter_get(iterator);
        mui_layout* layout = element->layout;
        switch (element->type) {

        case ELEMENT_TEXT:
            *buffer = text_shape(element->text.text, element->text.size, cursor, *buffer, bufferSize);
            printf("%i\n", *bufferSize);
            cursor.y += element->text.size;
            break;

        case ELEMENT_REGION:
            glGenBuffers(1, &element->region.id);
            vec2* regionBuffer = null;
            uint regionBufferSize = 0;

            compute_elements(element->region.children, cursor, &regionBuffer, &regionBufferSize);

            glBindBuffer(GL_ARRAY_BUFFER, element->region.id);
            glBufferData(GL_ARRAY_BUFFER, regionBufferSize, regionBuffer, GL_STATIC_DRAW);
            free(regionBuffer);

            element->region.count = regionBufferSize / sizeof(vec2) / 2;
            list_add(layout->regions, element);
            break;
        }
    }
    list_iter_cleanup(iterator);
}

void mui_layout_compute(mui_layout* layout) {
    if (layout->regions != null)
        list_cleanup(layout->regions);
    layout->regions = list_new();

    compute_elements(layout->elements, VEC2_ZERO, null, 0);
}

void layout_init(mui_window* window) {
    mui_layout* layout = malloc(sizeof(mui_layout));
    layout->elements = list_new();
    layout->window = window;
    layout->regions = null;
    layout->regionStack = stack_new(64);

    window->layout = layout;
    mui_layout_compute(layout);
}

mui_element* element_new(enum element_type type) {
    mui_element* element = malloc(sizeof(mui_element));
    mui_layout* layout = activeWindow->layout;

    element->type = type;
    element->layout = layout;
    if (layout->regionStack->count > 0) {
        mui_element* region = stack_peek(layout->regionStack);
        element->parent = region;
        list_add(region->region.children, element);
    } else {
        element->parent = null;
        list_add(layout->elements, element);
    }

    return element;
}

void mui_region_push() {
    mui_element* element = element_new(ELEMENT_REGION);
    mui_layout* layout = activeWindow->layout;

    element->region.children = list_new();

    stack_push(layout->regionStack, element);
}

void mui_region_pop() {
    mui_layout* layout = activeWindow->layout;
    stack_pop(layout->regionStack);
}

void mui_defaults() {
    settings.textSize = 15;
}

void mui_text(string text) {
    mui_element* element = element_new(ELEMENT_TEXT);
    mui_layout* layout = activeWindow->layout;

    element->text.text = text;
    element->text.size = settings.textSize;
}