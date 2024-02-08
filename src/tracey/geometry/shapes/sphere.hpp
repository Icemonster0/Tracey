#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <memory>
#include <optional>

#include "../../../glm.hpp"
#include "../../graphics/shader.hpp"
#include "../ray.hpp"
#include "../intersection.hpp"
#include "../shape.hpp"

namespace trc {

class Sphere : public Shape {
public:
    Sphere(glm::vec3 p_pos, float p_radius, Shader *p_shader);

    std::optional<Intersection> calc_ray_intersection(Ray ray) const;

private:
    glm::vec3 pos;
    float radius;
};

} /* trc */

#endif /* end of include guard: SPHERE_HPP */
