#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <memory>
#include <optional>

#include "../../glm.hpp"
#include "../graphics/shader.hpp"
#include "ray.hpp"
#include "intersection.hpp"

namespace trc {

class Shape {
public:
    Shape(Shader shader_object);
    Shape();

    virtual std::optional<Intersection> calc_ray_intersection(Ray ray);

    Shader *get_shader();
    void set_shader(Shader shader_object);

protected:
    std::unique_ptr<Shader> shader;
};

} /* trc */

#endif /* end of include guard: SHAPE_HPP */
