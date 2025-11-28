#include <string.h>
#include <mstd/common.h>

typedef struct {
    ulong size;
    void* data;
} mui_buffer;

mui_buffer* mui_buffer_new() {
    mui_buffer* buffer = malloc(sizeof(mui_buffer));
    buffer->size = 0;
    buffer->data = null;

    return buffer;
}

void mui_buffer_expand(mui_buffer* buffer, void* data, ulong size) {
    buffer->size += size;

    if (buffer->data == null)
        buffer->data = malloc(buffer->size);
    else
        buffer->data = realloc(buffer->data, buffer->size);

    memcpy(buffer->data + (buffer->size - size), data, size);
}

void mui_buffer_cleanup(mui_buffer* buffer) {
    free(buffer->data);
    free(buffer);
}