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
    mui_draw(window->layout);
}

bool mui_init() {
    if (!glfwInit()) {
        fprintf(stderr, "mui: failed to initialize glfw\n");
        return false;
    }

    glfwSwapInterval(1);

    windows = list_new();
    mui_window* window = mui_window_new(VEC2_ONE, "");

    if (!text_init("assets/NotoSansJP-Regular.ttf"))
        return false;

    if (!shader_init())
        return false;

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
        list_iterator* iterator = list_iter_new(windows);
        for (uint i = 0; i < windows->length; i++, list_iter_next(iterator)) {
            mui_window* window = list_iter_get(iterator);
            if (i == 0)
                continue;

            if (glfwWindowShouldClose(window->glfwWindow)) {
                mui_window_destroy(window);
                if (windows->length == 1)
                    shouldExit = true;
                continue;
            }

            glfwMakeContextCurrent(window->glfwWindow);
            glUseProgram(shaderProgramText.id);

            int width;
            int height;
            glfwGetWindowSize(window->glfwWindow, &width, &height);
            glViewport(0, 0, width, height);
            glUniform2f(shaderProgramText.screenUniform, width, height);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            mui_draw(window->layout);
            glfwSwapBuffers(window->glfwWindow);
        }
        list_iter_cleanup(iterator);

        glfwWaitEvents();
    }
}