#pragma once

#include <mstd/common.h>

extern uint shaderProgramRect;

bool mui_shader_program_new(uint* outProgram, string vert, string frag);
bool mui_shader_init();