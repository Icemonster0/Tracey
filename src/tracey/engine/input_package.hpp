#ifndef INPUT_PACKAGE_HPP
#define INPUT_PACKAGE_HPP

#include "../../glm.hpp"

namespace trc {

struct InputPackage {
    bool q, w, e, a, s, d;
    glm::vec2 delta_mouse;
};

} /* trc */

#endif /* end of include guard: INPUT_PACKAGE_HPP */
