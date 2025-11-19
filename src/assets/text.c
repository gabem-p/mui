#include <GLES3/gl32.h>
#include <harfbuzz/hb.h>
#include <freetype/freetype.h>
#include <harfbuzz/hb.h>
#include <mstd/common.h>
#include <mstd/types/primitives.h>

#define FONT_SIZE 16
#define ATLAS_SIZE 4096
const byte TEXTURE_MISSING[] = {[0 ... FONT_SIZE * FONT_SIZE - 1] = 255};

typedef struct {
    ushort width;
    ushort height;
    ushort bearingX;
    ushort bearingY;
} glyph_metrics;

glyph_metrics* metricsTable;

uint fontAtlasId;
hb_font_t* hbFont;

bool freetype_init(string filename) {
    FT_Library library;
    if (FT_Init_FreeType(&library) != 0) {
        fprintf(stderr, "mui: failed to initialize freetype\n");
        return false;
    }

    FT_Face face;
    if (FT_New_Face(library, filename, 0, &face) != 0) {
        fprintf(stderr, "mui: freetype: failed to read font \"%s\"\n", filename);
        return false;
    }
    FT_Set_Pixel_Sizes(face, 0, FONT_SIZE);

    glGenTextures(1, &fontAtlasId);
    glBindTexture(GL_TEXTURE_2D, fontAtlasId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8,
                 ATLAS_SIZE, ATLAS_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, null);

    metricsTable = malloc(face->num_glyphs * sizeof(glyph_metrics));

    for (uint i = 0; i < face->num_glyphs; i++) {
        bool error = FT_Load_Glyph(face, i, FT_LOAD_DEFAULT | FT_LOAD_RENDER);

        uint width = FONT_SIZE;
        uint height = FONT_SIZE;
        glyph_metrics metrics = {0};
        const byte* data = TEXTURE_MISSING;

        if (!error) {
            width = face->glyph->bitmap.width;
            height = face->glyph->bitmap.rows;
            data = face->glyph->bitmap.buffer;

            metrics.width = face->glyph->metrics.width / 64;
            metrics.height = face->glyph->metrics.height / 64;
            metrics.bearingX = face->glyph->metrics.horiBearingX / 64;
            metrics.bearingY = face->glyph->metrics.horiBearingY / 64;
        }

        metricsTable[i] = metrics;

        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        (i % (ATLAS_SIZE / FONT_SIZE)) * FONT_SIZE, (i / (ATLAS_SIZE / FONT_SIZE)) * FONT_SIZE,
                        width, height, GL_RED, GL_UNSIGNED_BYTE, data);
    }

    return true;
}

bool harfbuzz_init(string filename) {
    hb_blob_t* blob = hb_blob_create_from_file(filename);
    hb_face_t* face = hb_face_create(blob, 0);
    hbFont = hb_font_create(face);
    hb_font_set_scale(hbFont, FONT_SIZE * 64, FONT_SIZE * 64);

    return true;
}

bool mui_text_init(string filename) {
    if (!freetype_init(filename))
        return false;

    if (!harfbuzz_init(filename))
        return false;

    return true;
}

vec2* mui_text_shape(string text, uint* outSize) {
    hb_buffer_t* buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text, -1, 0, -1);
    hb_buffer_guess_segment_properties(buffer);

    hb_shape(hbFont, buffer, null, 0);

    uint glyphCount = 0;
    hb_glyph_info_t* glyphInfo = hb_buffer_get_glyph_infos(buffer, &glyphCount);
    hb_glyph_position_t* glyphPos = hb_buffer_get_glyph_positions(buffer, &glyphCount);

    uint size = glyphCount * 2 * 6 * sizeof(vec2);
    vec2* vertexBuffer = malloc(size);

    float cursorX = 0.0;
    float cursorY = FONT_SIZE;
    uint j = 0;
    for (uint i = 0; i < glyphCount; i++) {
        hb_codepoint_t glyph = glyphInfo[i].codepoint;
        glyph_metrics metrics = metricsTable[glyph];

        uint x = (glyph % (ATLAS_SIZE / FONT_SIZE)) * FONT_SIZE;
        uint y = (glyph / (ATLAS_SIZE / FONT_SIZE)) * FONT_SIZE;

        vertexBuffer[j++] = vec2(cursorX + metrics.bearingX, cursorY - metrics.bearingY);
        vertexBuffer[j++] = vec2(x, y);

        vertexBuffer[j++] = vec2(cursorX + metrics.bearingX, cursorY - metrics.bearingY + metrics.height);
        vertexBuffer[j++] = vec2(x, y + metrics.height);

        vertexBuffer[j++] = vec2(cursorX + metrics.bearingX + metrics.width, cursorY - metrics.bearingY + metrics.height);
        vertexBuffer[j++] = vec2(x + metrics.width, y + metrics.height);

        vertexBuffer[j++] = vec2(cursorX + metrics.bearingX + metrics.width, cursorY - metrics.bearingY + metrics.height);
        vertexBuffer[j++] = vec2(x + metrics.width, y + metrics.height);

        vertexBuffer[j++] = vec2(cursorX + metrics.bearingX + metrics.width, cursorY - metrics.bearingY);
        vertexBuffer[j++] = vec2(x + metrics.width, y);

        vertexBuffer[j++] = vec2(cursorX + metrics.bearingX, cursorY - metrics.bearingY);
        vertexBuffer[j++] = vec2(x, y);

        cursorX += (float)glyphPos[i].x_advance / 64;
    }

    *outSize = size;
    return vertexBuffer;
}