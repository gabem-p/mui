#pragma once

#include <mstd/common.h>
#include <mstd/types/primitives.h>

extern uint fontAtlasId;

bool text_init(string filename);
vec2* text_shape(string text, uint fontSize, vec2 position, uint* outSize);
