#include "glfw_callbacks.hpp"

namespace glfw_callbacks {

glm::vec2 CallbackState::get_delta_scroll() {
    glm::vec2 ret = delta_scroll;
    delta_scroll = glm::vec2 {0.f};
    return ret;
}

std::list<int> CallbackState::get_key_history() {
    std::list<int> ret = std::move(key_history);
    key_history = std::list<int> {};
    return ret;
}

void CallbackState::add_delta_scroll(glm::vec2 delta) {
    delta_scroll += delta;
}

void CallbackState::add_key(int key) {
    key_history.push_back(key);
}

CallbackState callback_state;

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    callback_state.add_delta_scroll({xoffset, yoffset});
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) callback_state.add_key(key);
}

void init_callbacks(GLFWwindow *window) {
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
}

} /* glfw_callbacks */
