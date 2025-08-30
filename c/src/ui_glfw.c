// src/ui_glfw.c
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_glfw_gl2.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

void run_gui() {
    if (!glfwInit()) {
        printf("❌ GLFW init failed\n");
        return;
    }

    GLFWwindow *win = glfwCreateWindow(800, 600, "Sentinel Vault", NULL, NULL);
    if (!win) {
        glfwTerminate();
        return;
    }

    nk_glfw3_init(win, NK_GLFW_INSTALL_CALLBACKS);
    struct nk_context *ctx = nk_glfw3_get_context();

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();
        nk_input_begin(ctx);

        nk_glfw3_new_frame();
        if (nk_begin(ctx, "Vault", nk_rect(50, 50, 700, 500),
                     NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
            nk_layout_row_static(ctx, 30, 200, 1);
            if (nk_button_label(ctx, "🔐 Инициализировать")) {
                handle_init("supersecret123");
            }
            if (nk_button_label(ctx, "🔓 Доступ к данным")) {
                handle_access("supersecret123");
            }
            if (nk_button_label(ctx, "🧨 Самоуничтожение")) {
                set_intrusion_trigger();
                printf("💥 Система активирована\n");
            }
        }
        nk_end(ctx);

        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(NK_ANTI_ALIASING_ON, 5000, 4096);
        glfwSwapBuffers(win);
    }

    nk_glfw3_shutdown();
    glfwTerminate();
}