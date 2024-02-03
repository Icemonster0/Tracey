#include "window_manager.hpp"

#include <cstdio>
#include <cstdlib>

namespace trc {

/* public */

WindowManager::WindowManager()
    : running(false) {}

WindowManager::WindowManager(glm::ivec2 window_size)
    : size(window_size), running(false) {

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

    running = true;

    printf("GLFW Info: %s\n", glfwGetVersionString());
}

void WindowManager::draw_frame(const Buffer<glm::vec3> *frame) {
    const glm::ivec2 frame_size = frame->get_size();
    /* glm::vec3 in memory looks like three consecutive floats */
    glDrawPixels(frame_size.x, frame_size.y, GL_RGB, GL_FLOAT, frame->get_data());
    glfwSwapBuffers(window);
}

void WindowManager::handle_events() {
    glfwPollEvents();

    // window resize
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    if (width != size.x || height != size.y) {
        size = glm::ivec2(width, height);
        glViewport(0, 0, width, height);
    }

    // keyboard input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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

void WindowManager::do_nothing() {
    printf("nothing\n");
}

} /* trc */
