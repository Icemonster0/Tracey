#include "ground_plane.hpp"

namespace trc {

GroundPlane::GroundPlane(float p_height, Shader p_shader)
    : Shape(p_shader), height(p_height) {}

std::optional<Intersection> GroundPlane::calc_ray_intersection(Ray ray) {

    if (ray.direction.y == 0) return std::optional<Intersection>();

    float const t = (height - ray.origin.y) / ray.direction.y;
    if (t < TRC_RAY_CLIP_EPSILON) return std::optional<Intersection>();

    glm::vec3 point = ray.get_point(t);

    return std::make_optional<Intersection> (
        point, // pos
        glm::vec3 {0.f, 1.f, 0.f}, // normal
        glm::fract(point.xz()), // tex_coord
        t, // distance
        shader.get() // shader
    );
}

} /* trc */
