#pragma once

#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include "src/render/buffer.h"

extern uint fontAtlasId;

bool text_init(string filename);
void text_shape(string text, uint fontSize, vec2 position, mui_buffer* buffer);