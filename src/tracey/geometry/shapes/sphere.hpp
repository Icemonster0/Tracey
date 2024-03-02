#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <memory>
#include <optional>

#include "../../../lib/glm.hpp"
#include "../../graphics/shader.hpp"
#include "../ray.hpp"
#include "../intersection.hpp"
#include "../shape.hpp"

namespace trc {

class Sphere : public Shape {
public:
    Sphere(glm::vec3 p_pos, float p_radius, Shader *p_shader, Material *p_material);

    std::optional<Intersection> calc_ray_intersection(Ray ray) const;
    Box calc_bounding_box() const;
    glm::vec3 center() const;

private:
    glm::vec3 pos;
    float radius;
};

} /* trc */

#endif /* end of include guard: SPHERE_HPP */
