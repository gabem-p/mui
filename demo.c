#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include "mui.h"

void layout(mui_window* window) {
    mui_region {
        mui_text("hello mui - 恋をして!");
        mui_text("hello again!");
        mui_text("and now were tiny :(");
    }
}

int main(int argc, string argv[]) {
    if (!mui_init())
        return -1;
    printf("gl version: %s\n", glGetString(GL_VERSION));

    mui_window* window = mui_window_new(vec2(1000, 600), "mui");
    mui_window_layout(window, layout);

    mui_loop();

    mui_destroy();
    return EXIT_SUCCESS;
}