#include <GLES3/gl32.h>
#include <mstd/common.h>
#include "src/assets/text.h"
#include "src/assets/shader.h"

uint textBuffer;
uint textBufferCount;
uint vao;

void mui_debug_draw_atlas() {
    glBindTexture(GL_TEXTURE_2D, fontAtlasId);
    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
    glUniform1i(shaderProgramRect.samplerUniform, 0);
    glUniform1ui(shaderProgramRect.textUniform, true);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(vec2), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(vec2), (void*)(sizeof(vec2)));

    glDrawArrays(GL_TRIANGLES, 0, textBufferCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}