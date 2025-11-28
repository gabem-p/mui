#pragma once;

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
            uint id;
            uint count;
            list* children;
        } region;
    };

} mui_element;

typedef struct {
    list* elements;
    void* window;

    list* regions;
    stack* regionStack;
} mui_layout;

void mui_layout_compute(mui_layout* layout);
void mui_defaults();

void mui_text(string text);
void mui_text_size(uint size);

void mui_region_push();
void mui_region_pop();
#define mui_region for(uint i = 0, mui_region_push(); i++ == 0; mui_region_pop())