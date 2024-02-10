#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <optional>

#include "../../lib/glm.hpp"
#include "../graphics/shader.hpp"
#include "ray.hpp"
#include "intersection.hpp"

namespace trc {

class Shape {
public:
    Shape(Shader *shader_ptr);

    virtual std::optional<Intersection> calc_ray_intersection(Ray ray) const;

    Shader *get_shader();
    void set_shader(Shader *shader_ptr);

protected:
    Shader *shader;
};

} /* trc */

#endif /* end of include guard: SHAPE_HPP */
