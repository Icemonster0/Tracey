#include "sphere.hpp"

#include "../../util/math_util.hpp"

namespace trc {

Sphere::Sphere(glm::vec3 p_pos, float p_radius, Shader *p_shader, Material *p_material)
    : Shape(p_shader, p_material), pos(p_pos), radius(p_radius) {}

std::optional<Intersection> Sphere::calc_ray_intersection(Ray ray) const {
    // calculate intersection
    std::optional<float> t = math::intersect_sphere(ray, pos, radius);
    if (!t) return std::optional<Intersection>();

    // calculate intersection parameters
    const glm::vec3 point = ray.get_point(t.value());
    const glm::vec3 normal = math::normalize(point - pos);

    // texture coordinates
    glm::vec2 tex_coord;
    if (normal.z == 0.f) {
        tex_coord.s = 0.f;
    } else {
        tex_coord.s = atanf(normal.x / normal.z) / 6.2831853f;
        if (normal.z >= 0.f) tex_coord.s += 0.75f;
        else tex_coord.s += 0.25f;
    }
    float radius_at_height = glm::length(normal.xz());
    if (radius_at_height == 0.f) {
        tex_coord.t = 0.f;
    } else {
        tex_coord.t = atanf(normal.y / radius_at_height) / 3.1415927f + 0.5f;
    }

    // tangent and bitangent
    glm::vec3 tan = glm::cross(glm::vec3 {0.f, 1.f, 0.f}, normal);
    glm::vec3 bitan = glm::cross(normal, tan);

    return std::make_optional<Intersection> (
        point, // pos
        normal,
        tan,
        bitan,
        tex_coord,
        material,
        t.value(), // distance
        shader
    );
}

Box Sphere::calc_bounding_box() const {
    return Box {
        pos - glm::vec3 {radius},
        pos + glm::vec3 {radius}
    };
}

glm::vec3 Sphere::center() const {
    return pos;
}

} /* trc */
