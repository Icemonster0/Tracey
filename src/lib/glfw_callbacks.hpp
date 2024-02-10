#ifndef GLFW_CALLBACKS_HPP
#define GLFW_CALLBACKS_HPP

#include <list>

#include "glfw.hpp"
#include "glm.hpp"

namespace glfw_callbacks {

struct CallbackState {
    glm::vec2 get_delta_scroll();
    std::list<int> get_key_history();

    void add_delta_scroll(glm::vec2 delta);
    void add_key(int key);

private:
    glm::vec2 delta_scroll {0.f};
    std::list<int> key_history;

} extern callback_state;

void init_callbacks(GLFWwindow *window);

} /* glfw_callbacks */

#endif /* end of include guard: GLFW_CALLBACKS_HPP */
