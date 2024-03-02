#ifndef BOX_HPP
#define BOX_HPP

#include "../../lib/glm.hpp"

#include "ray.hpp"

namespace trc {

struct Box {
    Box(glm::vec3 p_pmin, glm::vec3 p_pmax);
    Box();

    glm::vec3 pmin;
    glm::vec3 pmax;

    bool intersect_ray(Ray ray) const;
    void include(Box other);
    unsigned char max_dimension() const;
    glm::vec3 center() const;
};

} /* trc */

#endif /* end of include guard: BOX_HPP */
