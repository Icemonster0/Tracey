#include "box.hpp"

#include <limits>

namespace trc {

Box::Box(glm::vec3 p_pmin, glm::vec3 p_pmax)
    : pmin(p_pmin), pmax(p_pmax) {}

Box::Box()
    : pmin(glm::vec3 {0.f}), pmax(glm::vec3 {0.f}) {}

bool Box::intersect_ray(Ray ray) const {
    // referene: https://tavianator.com/2022/ray_box_boundary.html

    float tmin = 0.f, tmax = std::numeric_limits<float>::infinity();
    float t1, t2;

    glm::vec3 inv_dir = 1.f / ray.direction;

    t1 = (pmin.x - ray.origin.x) * inv_dir.x;
    t2 = (pmax.x - ray.origin.x) * inv_dir.x;
    tmin = std::min(std::max(t1, tmin), std::max(t2, tmin));
    tmax = std::max(std::min(t1, tmax), std::min(t2, tmax));

    t1 = (pmin.y - ray.origin.y) * inv_dir.y;
    t2 = (pmax.y - ray.origin.y) * inv_dir.y;
    tmin = std::min(std::max(t1, tmin), std::max(t2, tmin));
    tmax = std::max(std::min(t1, tmax), std::min(t2, tmax));

    t1 = (pmin.z - ray.origin.z) * inv_dir.z;
    t2 = (pmax.z - ray.origin.z) * inv_dir.z;
    tmin = std::min(std::max(t1, tmin), std::max(t2, tmin));
    tmax = std::max(std::min(t1, tmax), std::min(t2, tmax));

    return tmin <= tmax;
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
