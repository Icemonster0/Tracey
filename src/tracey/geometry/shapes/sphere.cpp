#include "sphere.hpp"

#include "../../util/math_util.hpp"

namespace trc {

Sphere::Sphere(glm::vec3 p_pos, float p_radius, Shader *p_shader, Material *p_material)
    : Shape(p_shader, p_material), pos(p_pos), radius(p_radius) {}

std::optional<Intersection> Sphere::calc_ray_intersection(Ray ray) const {
    // translate ray to object space
    ray.origin -= pos;

    // coefficients for the quadratic equation at² + bt + c = 0
    const float a = ray.direction.x*ray.direction.x + ray.direction.y*ray.direction.y + ray.direction.z*ray.direction.z;
    const float b = 2 * (ray.direction.x*ray.origin.x + ray.direction.y*ray.origin.y + ray.direction.z*ray.origin.z);
    const float c = ray.origin.x*ray.origin.x + ray.origin.y*ray.origin.y + ray.origin.z*ray.origin.z - radius*radius;

    // solve equation
    const float discriminant = b*b - 4*a*c;

    if (discriminant < 0.f) return std::optional<Intersection>();

    const float inv_denominator = 1.f / (2*a);

    float t;

    if (discriminant == 0) {
        t = -b * inv_denominator;
        if (t < TRC_RAY_CLIP_EPSILON) return std::optional<Intersection>();
    }
    else {
        float sqrt_discriminant = sqrtf(discriminant);
        float t1 = (-b + sqrt_discriminant) * inv_denominator;
        float t2 = (-b - sqrt_discriminant) * inv_denominator;
        if (t1 < TRC_RAY_CLIP_EPSILON) {
            if (t2 < TRC_RAY_CLIP_EPSILON) return std::optional<Intersection>();
            else t = t2;
        } else if (t2 < TRC_RAY_CLIP_EPSILON) t = t1;
        else t = std::min(t1, t2);
    }

    // calculate intersection parameters
    const glm::vec3 point = ray.get_point(t);
    const glm::vec3 normal = math::normalize(point);
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

    return std::make_optional<Intersection> (
        point + pos, // pos
        normal,
        tex_coord,
        material,
        t, // distance
        shader
    );
}

} /* trc */
