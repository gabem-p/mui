#pragma once

#include <mstd/common.h>
#include <mstd/types/primitives.h>

extern uint fontAtlasId;

bool text_init(string filename);
uint text_shape(string text, uint fontSize, vec2 position, vec2** vertexBuffer, uint bufferSize);