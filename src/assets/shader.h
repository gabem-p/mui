#pragma once

#include <mstd/common.h>

typedef struct {
    uint id;
    uint screenUniform;
    uint samplerUniform;
    uint colorUniform;
} program_text;
extern program_text shaderProgramText;

bool shader_init();