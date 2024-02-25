#include "ground_plane.hpp"

namespace trc {

GroundPlane::GroundPlane(float p_height, Shader *p_shader, Material *p_material)
    : Shape(p_shader, p_material), height(p_height) {}

std::optional<Intersection> GroundPlane::calc_ray_intersection(Ray ray) const {

    if (ray.direction.y == 0) return std::optional<Intersection>();

    float const t = (height - ray.origin.y) / ray.direction.y;
    if (t < TRC_RAY_CLIP_EPSILON) return std::optional<Intersection>();

    glm::vec3 point = ray.get_point(t);

    return std::make_optional<Intersection> (
        point, // pos
        glm::vec3 {0.f, 1.f, 0.f}, // normal
        glm::vec3 {1.f, 0.f, 0.f}, // tangent
        glm::vec3 {0.f, 0.f, 1.f}, // bitangent
        glm::fract(point.xz()), // tex_coord
        material, // material
        t, // distance
        shader // shader
    );
}

} /* trc */
