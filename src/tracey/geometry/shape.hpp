#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <optional>

#include "../../lib/glm.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/material.hpp"
#include "ray.hpp"
#include "intersection.hpp"

namespace trc {

class Shape {
public:
    Shape(Shader *shader_ptr, std::shared_ptr<Material> p_material);

    virtual std::optional<Intersection> calc_ray_intersection(Ray ray) const;

    Shader *get_shader();
    void set_shader(Shader *shader_ptr);
    std::shared_ptr<Material> get_material();
    void set_material(std::shared_ptr<Material> mat);

protected:
    Shader *shader;
    std::shared_ptr<Material> material;
};

} /* trc */

#endif /* end of include guard: SHAPE_HPP */
