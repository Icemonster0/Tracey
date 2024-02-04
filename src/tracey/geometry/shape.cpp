#include "shape.hpp"

namespace trc {

Shape::Shape(Shader shader_object) {
    set_shader(shader_object);
}

Shape::Shape() {
    set_shader(Shader {});
}

std::optional<Intersection> Shape::calc_ray_intersection(Ray ray) {
    return std::optional<Intersection>();
}

Shader *Shape::get_shader() {
    return shader.get();
}

void Shape::set_shader(Shader shader_object) {
    shader = std::make_unique<Shader>(shader_object);
}

} /* trc */
