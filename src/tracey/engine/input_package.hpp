#ifndef INPUT_PACKAGE_HPP
#define INPUT_PACKAGE_HPP

#include "../../lib/glm.hpp"

namespace trc {

struct InputPackage {
    bool q, w, e, a, s, d, r, p;
    glm::vec2 delta_mouse;
    glm::vec2 delta_scroll;
};

} /* trc */

#endif /* end of include guard: INPUT_PACKAGE_HPP */
