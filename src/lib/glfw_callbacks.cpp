#include "glfw_callbacks.hpp"

namespace glfw_callbacks {

glm::vec2 CallbackState::get_delta_scroll() {
    glm::vec2 ret = delta_scroll;
    delta_scroll = glm::vec2 {0.f};
    return ret;
}

void CallbackState::add_delta_scroll(glm::vec2 delta) {
    delta_scroll += delta;
}

CallbackState callback_state;

void init_callbacks(GLFWwindow *window) {
    glfwSetScrollCallback(window, scroll_callback);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    callback_state.add_delta_scroll({xoffset, yoffset});
}

} /* glfw_callbacks */
