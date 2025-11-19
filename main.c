#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <mstd/common.h>
#include <mstd/types/primitives.h>
#include "mui.h"

// void layout(mui_window* window) {
//     mui_text("hello mui - 恋をして!");
// }

int main(int argc, string argv[]) {
    if (!mui_init())
        return -1;

    printf("gl version: %s\n", glGetString(GL_VERSION));

    mui_loop();

    mui_destroy();
    return EXIT_SUCCESS;
}