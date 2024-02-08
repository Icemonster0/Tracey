#include "ray.hpp"

namespace trc {

Ray::Ray(glm::vec3 o, glm::vec3 d, RayType t, int i)
    : origin(o), direction(d), type(t), index(i) {}

glm::vec3 Ray::get_point(float t) {
    return origin + direction * t;
}

} /* trc */
