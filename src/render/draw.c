#include <GLES3/gl32.h>
#include "src/window.h"
#include <mstd/common.h>
#include "src/assets/text.h"
#include "src/assets/shader.h"
#include "src/layout.h"

void mui_draw(mui_layout* layout) {
    list_iterator* iterator = list_iter_new(layout->regions);
    for (uint i = 0; i < layout->regions->length; i++, list_iter_next(iterator)) {
        mui_element* region = list_iter_get(iterator);

        glBindVertexArray(((mui_window*)layout->window)->vao);
        glBindTexture(GL_TEXTURE_2D, fontAtlasId);
        glBindBuffer(GL_ARRAY_BUFFER, region->region.id);
        glUniform1i(shaderProgramText.samplerUniform, 0);
        glUniform4f(shaderProgramText.colorUniform, 0, 0, 0, 1);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * sizeof(vec2), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 2 * sizeof(vec2), (void*)(sizeof(vec2)));

        glDrawArrays(GL_TRIANGLES, 0, region->region.count);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
    }
    list_iter_cleanup(iterator);
}