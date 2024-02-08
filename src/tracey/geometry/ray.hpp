#ifndef RAY_HPP
#define RAY_HPP

#include "../../glm.hpp"

#define TRC_RAY_CLIP_EPSILON 0.01f
#define TRC_RAY_MAX_BOUNCES 10

namespace trc {

enum RayType {
    TRC_CAMERA_RAY,
    TRC_SHADOW_RAY,
    TRC_COMMON_RAY
};

struct Ray {
    Ray(glm::vec3 o, glm::vec3 d, RayType t = TRC_COMMON_RAY, int i = 0);

    glm::vec3 get_point(float t);

    glm::vec3 origin;
    glm::vec3 direction;
    RayType type;
    int index;
};

} /* trc */

#endif /* end of include guard: RAY_HPP */
