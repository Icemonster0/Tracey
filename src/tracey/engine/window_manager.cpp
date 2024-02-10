#include "window_manager.hpp"

#include <cstdio>
#include <cstdlib>

#include "../../lib/glfw_callbacks.hpp"

namespace trc {

/* public */

WindowManager::WindowManager()
    : running(false) {}

WindowManager::WindowManager(glm::ivec2 window_size)
    : size(window_size), running(false), is_mouse_captured(false), last_enter_key_press_state(false), update_required(false) {

    if(!glfwInit()) {
        fprintf(stderr, "Failed to init glfw\n");
        exit(-1);
    }

    window = glfwCreateWindow(size.x, size.y, "Tracey Engine", nullptr, nullptr);
    if(!window) {
        fprintf(stderr, "Failed to create glfw window\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
    is_mouse_captured = true;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    glfw_callbacks::init_callbacks(window);

    running = true;

    printf("GLFW Info: %s\n", glfwGetVersionString());
}

void WindowManager::draw_frame(const Buffer<glm::vec3> *frame) {
    const glm::ivec2 frame_size = frame->get_size();
    /* glm::vec3 in memory looks like three consecutive floats */
    glDrawPixels(frame_size.x, frame_size.y, GL_RGB, GL_FLOAT, frame->get_data());
    glfwSwapBuffers(window);
}

InputPackage WindowManager::handle_events() {
    glfwPollEvents();

    update_required = false;

    // window resize
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (width != size.x || height != size.y) {
        size = glm::ivec2(width, height);
        glViewport(0, 0, width, height);
        update_required = true;
    }

    // keyboard input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (!last_enter_key_press_state) {
            last_enter_key_press_state = true;
            if (is_mouse_captured) {
                is_mouse_captured = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else {
                is_mouse_captured = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
    else {
        last_enter_key_press_state = false;
    }

    // mouse movement
    glm::dvec2 new_mouse_pos;
    glfwGetCursorPos(window, &new_mouse_pos.x, &new_mouse_pos.y);
    glm::vec2 delta_mouse_pos = new_mouse_pos - mouse_pos;
    mouse_pos = new_mouse_pos;

    // misc input info
    InputPackage pack;
    pack.q = (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS);
    pack.w = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    pack.e = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
    pack.a = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    pack.s = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    pack.d = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    if (is_mouse_captured) pack.delta_mouse = delta_mouse_pos;
    else pack.delta_mouse = glm::vec2 {0.f};
    if (is_mouse_captured) pack.delta_scroll = glfw_callbacks::callback_state.get_delta_scroll();
    else pack.delta_scroll = glm::vec2 {0.f};

    if (pack.q || pack.w || pack.e || pack.a || pack.s || pack.d || pack.delta_mouse.x != 0.f || pack.delta_mouse.y != 0.f) {
        update_required = true;
    }

    return pack;
}

void WindowManager::close() {
    running = false;
    glfwTerminate();
}

bool WindowManager::window_should_close() {
    return glfwWindowShouldClose(window);
}

glm::ivec2 WindowManager::get_size() {
    return size;
}

bool WindowManager::is_running() {
    return running;
}

bool WindowManager::is_update_required() {
    return update_required;
}

} /* trc */
