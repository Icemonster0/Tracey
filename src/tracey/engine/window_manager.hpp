#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include "../../glfw.hpp"
#include "../../glm.hpp"

#include "../graphics/buffer.hpp"
#include "input_package.hpp"

namespace trc {


class WindowManager {
public:
    WindowManager();
    WindowManager(glm::ivec2 size);

    void draw_frame(const Buffer<glm::vec3> *frame);
    InputPackage handle_events();
    void close();

    bool window_should_close();
    glm::ivec2 get_size();
    bool is_running();

    void do_nothing();

private:
    GLFWwindow *window;
    glm::ivec2 size;
    bool running;
    glm::dvec2 mouse_pos;
    bool is_mouse_captured;
    bool last_enter_key_press_state;
};

} /* trc */

#endif /* end of include guard: WINDOW_MANAGER_HPP */
