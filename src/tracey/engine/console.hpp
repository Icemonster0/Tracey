#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "../../glm.hpp"

namespace trc {

class Console {
public:
    Console();
    Console(float refresh_rate);

    void print(float delta_t, glm::ivec2 window_size);
    void close();

private:
    float frequency;
    float print_cooldown;
};

} /* trc */

#endif /* end of include guard: CONSOLE_HPP */
