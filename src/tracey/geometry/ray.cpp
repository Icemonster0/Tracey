#include "ray.hpp"

namespace trc {

Ray::Ray(glm::vec3 o, glm::vec3 d)
    : origin(o), direction(d) {}

glm::vec3 Ray::get_point(float t) {
    return origin + direction * t;
}

} /* trc */
