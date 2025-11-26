#include <GLES3/gl32.h>
#include <string.h>
#include <mstd/common.h>

string str_from_shader_type(GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER:
        return "vertex";
    case GL_FRAGMENT_SHADER:
        return "fragment";
    case GL_GEOMETRY_SHADER:
        return "geometry";
    default:
        return null;
    }
}

typedef struct {
    uint shader;
    bool success;
} shader_compile_result;

shader_compile_result compile_shader(GLenum type, string path) {
    uint shader = glCreateShader(type);
    FILE* file = fopen(path, "r");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    string source = malloc(size);
    fread(source, size, 1, file);

    fclose(file);

    glShaderSource(shader, 1, (const string*)&source, null);
    glCompileShader(shader);

    bool success = true;
    glGetShaderiv(shader, GL_COMPILE_STATUS, (int*)&success);
    if (!success) {
        int length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        string infoLog = calloc(1, length);
        glGetShaderInfoLog(shader, length, null, infoLog);
        fprintf(stderr, "mui: shader compilation error (%s):\n%s\n", str_from_shader_type(type), infoLog);
    }

    return (shader_compile_result){shader, success};
}

bool shader_program_new(uint* outProgram, string vert, string frag, string geom) {
    uint program = glCreateProgram();

#define compile(type, arg) { \
    if(arg != null) { \
        shader_compile_result result = compile_shader(type, arg); \
        glAttachShader(program, result.shader); \
    } \
}

    compile(GL_VERTEX_SHADER, vert);
    compile(GL_FRAGMENT_SHADER, frag);
    compile(GL_GEOMETRY_SHADER, geom);

    bool success = true;
    glGetProgramiv(program, GL_COMPILE_STATUS, (int*)&success);
    if (!success) {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        string infoLog = calloc(1, length);
        glGetProgramInfoLog(program, length, null, infoLog);
        fprintf(stderr, "mui: program linking error:\n%s\n", infoLog);

        return false;
    }

    glLinkProgram(program);
    *outProgram = program;
    return true;
}

typedef struct {
    uint id;
    uint screenUniform;
    uint samplerUniform;
    uint textUniform;
} program_rect;
program_rect shaderProgramRect;

bool shader_init() {
#define error(name) { \
    fprintf(stderr, "mui: shader program \"%s\" failed to compile\n", name); \
}

    if (!shader_program_new(&shaderProgramRect.id,
                            "assets/shader/rect.vert",
                            "assets/shader/rect.frag",
                            null)) {
        error("rect");
        return false;
    }
    shaderProgramRect.screenUniform = glGetUniformLocation(shaderProgramRect.id, "screen");
    shaderProgramRect.samplerUniform = glGetUniformLocation(shaderProgramRect.id, "texSampler");
    shaderProgramRect.textUniform = glGetUniformLocation(shaderProgramRect.id, "text");

    return true;
}