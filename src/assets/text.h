#pragma once

#include <mstd/common.h>
#include <mstd/types/primitives.h>

extern uint fontAtlasId;

bool mui_text_init(string filename);
vec2* mui_text_shape(string text, uint* outSize);