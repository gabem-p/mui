#pragma once

#include <mstd/common.h>

typedef struct {
    ulong size;
    void* data;
} mui_buffer;

mui_buffer* mui_buffer_new();
void mui_buffer_expand(mui_buffer* buffer, void* data, ulong size);
void mui_buffer_cleanup(mui_buffer* buffer);