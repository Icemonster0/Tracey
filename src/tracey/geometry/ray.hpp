#ifndef RAY_HPP
#define RAY_HPP

#include "../../glm.hpp"

#define TRC_RAY_CLIP_EPSILON 0.01f

namespace trc {

struct Ray {
    Ray(glm::vec3 o, glm::vec3 d);

    glm::vec3 get_point(float t);

    glm::vec3 origin;
    glm::vec3 direction;
};

} /* trc */

#endif /* end of include guard: RAY_HPP */
