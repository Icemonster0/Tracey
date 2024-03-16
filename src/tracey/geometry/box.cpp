#include "box.hpp"

#include <limits>

#include "../util/math_util.hpp"

namespace trc {

Box::Box(glm::vec3 p_pmin, glm::vec3 p_pmax)
    : pmin(p_pmin), pmax(p_pmax) {}

Box::Box()
    : pmin(glm::vec3 {0.f}), pmax(glm::vec3 {0.f}) {}

bool Box::intersect_ray(Ray ray) const {
    glm::vec2 t = math::intersect_aabb(ray, pmin, pmax);
    return t.x <= t.y;
}

void Box::include(Box other) {
    pmin = glm::min(pmin, other.pmin);
    pmax = glm::max(pmax, other.pmax);
}

unsigned char Box::max_dimension() const {
    glm::vec3 dim = glm::abs(pmax - pmin);
    if (dim.x > dim.y)
        if (dim.x > dim.z) return 0;
        else return 2;
    else
        if (dim.y > dim.z) return 1;
        else return 2;
}

glm::vec3 Box::center() const {
    return (pmin + pmax) / 2.f;
}

} /* trc */
