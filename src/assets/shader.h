#pragma once

#include <mstd/common.h>

typedef struct {
    uint id;
    uint screenUniform;
    uint samplerUniform;
    uint textUniform;
} program_rect;
extern program_rect shaderProgramRect;

bool shader_init();