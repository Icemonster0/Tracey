#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "../../glm.hpp"

namespace trc {

class Console {
public:
    Console();
    Console(float refresh_rate);

    void print(float delta_t, glm::ivec2 window_size, glm::vec3 view_pos, float yaw, float pitch, float fov);
    void close();

private:
    float frequency;
    float print_cooldown;
};

} /* trc */

#endif /* end of include guard: CONSOLE_HPP */
