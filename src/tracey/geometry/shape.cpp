#include "shape.hpp"

namespace trc {

Shape::Shape(Shader *shader_ptr, Material *p_material)
    : shader(shader_ptr), material(p_material) {}

std::optional<Intersection> Shape::calc_ray_intersection(Ray ray) const {
    return std::optional<Intersection>();
}

Box Shape::calc_bounding_box() const {
    return Box {};
}

glm::vec3 Shape::center() const {
    return glm::vec3 {};
}

Shader *Shape::get_shader() {
    return shader;
}

void Shape::set_shader(Shader *shader_ptr) {
    shader = shader_ptr;
}

Material *Shape::get_material() {
    return material;
}

void Shape::set_material(Material *mat) {
    material = mat;
}

} /* trc */
