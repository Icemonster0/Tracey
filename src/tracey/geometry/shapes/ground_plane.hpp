#ifndef GROUND_PLANE_HPP
#define GROUND_PLANE_HPP

#include <memory>
#include <optional>

#include "../../../lib/glm.hpp"
#include "../../graphics/shader.hpp"
#include "../ray.hpp"
#include "../intersection.hpp"
#include "../shape.hpp"

namespace trc {

class GroundPlane : public Shape {
public:
    GroundPlane(float p_height, Shader *p_shader, Material *p_material);

    std::optional<Intersection> calc_ray_intersection(Ray ray) const;
    Box calc_bounding_box() const;
    glm::vec3 center() const;

private:
    float height;
};

} /* trc */

#endif /* end of include guard: GROUND_PLANE_HPP */
