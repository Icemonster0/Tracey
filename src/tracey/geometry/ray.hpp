#ifndef RAY_HPP
#define RAY_HPP

#include "../../lib/glm.hpp"

#define TRC_RAY_CLIP_EPSILON 0.0001f
#define TRC_RAY_MAX_BOUNCES 3
#define TRC_INDIRECT_LIGHT_CLAMP 10.f
#define TRC_SPECULAR_OPTIMIZE_TRESHOLD 0.2f

namespace trc {

enum RayType {
    TRC_CAMERA_RAY,
    TRC_SHADOW_RAY,
    TRC_DIFFUSE_RAY,
    TRC_SPECULAR_RAY,
    TRC_TRANSMISSION_RAY,
    TRC_TRANSPARENT_RAY,
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
