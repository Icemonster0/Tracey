#include "shape.hpp"

namespace trc {

Shape::Shape(Shader *shader_ptr)
    : shader(shader_ptr) {}

std::optional<Intersection> Shape::calc_ray_intersection(Ray ray) const {
    return std::optional<Intersection>();
}

Shader *Shape::get_shader() {
    return shader;
}

void Shape::set_shader(Shader *shader_ptr) {
    shader = shader_ptr;
}

} /* trc */
