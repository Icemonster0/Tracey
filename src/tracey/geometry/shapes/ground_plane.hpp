#ifndef GROUND_PLANE_HPP
#define GROUND_PLANE_HPP

#include <memory>
#include <optional>

#include "../../../glm.hpp"
#include "../../graphics/shader.hpp"
#include "../ray.hpp"
#include "../intersection.hpp"
#include "../shape.hpp"

namespace trc {

class GroundPlane : public Shape {
public:
    GroundPlane(float p_height, Shader p_shader);

    std::optional<Intersection> calc_ray_intersection(Ray ray);

private:
    float height;
};

} /* trc */

#endif /* end of include guard: GROUND_PLANE_HPP */
