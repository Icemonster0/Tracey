#ifndef GLFW_CALLBACKS_HPP
#define GLFW_CALLBACKS_HPP

#include "glfw.hpp"
#include "glm.hpp"

namespace glfw_callbacks {

struct CallbackState {
    glm::vec2 get_delta_scroll();

    void add_delta_scroll(glm::vec2 delta);

private:
    glm::vec2 delta_scroll {0.f};

} extern callback_state;

void init_callbacks(GLFWwindow *window);

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

} /* glfw_callbacks */

#endif /* end of include guard: GLFW_CALLBACKS_HPP */
