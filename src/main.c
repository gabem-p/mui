#include <GLFW/glfw3.h>
#include <GLES3/gl32.h>
#include <mstd/common.h>
#include <mstd/types/list.h>
#include "src/window.h"
#include "src/render/draw.h"
#include "src/assets/shader.h"
#include "src/assets/text.h"

static bool shouldExit = false;

void loop(mui_window* window) {
    mui_debug_draw_atlas();
}

bool mui_init() {
    if (!glfwInit()) {
        fprintf(stderr, "mui: failed to initialize glfw\n");
        return false;
    }

    glfwSwapInterval(1);

    windows = list_new();
    mui_window* window = mui_window_new(vec2(1000, 600), "mui");
    mui_window_set_loop(window, loop);

    if (!text_init("assets/NotoSansJP-Regular.ttf"))
        return false;

    if (!shader_init())
        return false;

    glUseProgram(shaderProgramRect.id);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glClearColor(1, 1, 1, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    uint size;
    vec2* buffer = text_shape("hello mui - 恋をして!", 15, VEC2_ZERO, &size);
    textBufferCount = size / sizeof(vec2) / 2;
    glGenBuffers(1, &textBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

    return true;
}

void mui_destroy() {
    glfwTerminate();
    list_iterator* iterator = list_iter_new(windows);
    for (uint i = 0; i < windows->length; i++, list_iter_next(iterator)) {
        mui_window* window = list_iter_get(iterator);
        glfwDestroyWindow(window->glfwWindow);
        free(window);
    }
    list_iter_cleanup(iterator);
    list_cleanup(windows);
}

void mui_exit() {
    shouldExit = true;
}

void mui_loop() {
    shouldExit = false;
    while (!shouldExit) {
        glfwPollEvents();

        list_iterator* iterator = list_iter_new(windows);
        for (uint i = 0; i < windows->length; i++, list_iter_next(iterator)) {
            mui_window* window = list_iter_get(iterator);

            if (glfwWindowShouldClose(window->glfwWindow)) {
                mui_window_destroy(window);
                if (windows->length == 0)
                    shouldExit = true;
                continue;
            }

            if (window->loop == null)
                continue;
            glfwMakeContextCurrent(window->glfwWindow);

            int width;
            int height;
            glfwGetWindowSize(window->glfwWindow, &width, &height);
            glViewport(0, 0, width, height);
            glUniform2f(shaderProgramRect.screenUniform, width, height);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            window->loop(window);
            glfwSwapBuffers(window->glfwWindow);
        }
        list_iter_cleanup(iterator);
    }
}