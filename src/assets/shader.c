#include <GLES3/gl32.h>
#include <string.h>
#include <mstd/common.h>

string str_from_shader_type(GLenum type) {
    switch (type) {
    case GL_VERTEX_SHADER:
        return "vertex";
    case GL_FRAGMENT_SHADER:
        return "fragment";
    default:
        return null;
    }
}

typedef struct {
    uint shader;
    bool success;
} shader_compile_result;

shader_compile_result compile_shader(GLenum type, FILE* file) {
    uint shader = glCreateShader(type);

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

bool mui_shader_program_new(uint* outProgram, FILE* vert, FILE* frag) {
    uint program = glCreateProgram();

#define compile(type, arg) { \
    shader_compile_result result = compile_shader(type, arg); \
    glAttachShader(program, result.shader); \
}

    compile(GL_VERTEX_SHADER, vert);
    compile(GL_FRAGMENT_SHADER, frag);

    bool success = true;
    glGetProgramiv(program, GL_COMPILE_STATUS, (int*)&success);
    if (!success) {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        string infoLog = calloc(1, length);
        glGetProgramInfoLog(program, length, null, infoLog);
        fprintf(stderr, "mui: program linking error:\n%s\n", infoLog);
    }

    glLinkProgram(program);
    *outProgram = program;
    return success;
}

uint shaderProgramRect;

bool mui_shader_init() {
#define error(name) fprintf(stderr, "mui: shader program \"%s\" failed to compile\n", name)

    if (!mui_shader_program_new(&shaderProgramRect, fopen("assets/shader/rect.vert", "r"), fopen("assets/shader/rect.frag", "r"))) {
        error("rect");
        return false;
    }
    return true;
}