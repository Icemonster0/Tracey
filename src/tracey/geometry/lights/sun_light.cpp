#include "sun_light.hpp"

#include <limits>

namespace trc {

SunLight::SunLight(glm::vec3 p_direction, glm::vec3 p_color, float p_intensity, float p_angle)
    : Light(glm::vec3 {0.f}, p_color, p_intensity), half_angle(p_angle / 2.f), direction(glm::normalize(p_direction)),
      random_distrib(std::uniform_real_distribution<float> {0.f, 6.283185307f}) {}

LightSampleData SunLight::sample(glm::vec3 shading_point, RNG *rng) {
    const int c = 10000;
    float t = random_distrib(*rng);

    glm::vec3 sphere_offset {
        sin(c*c*t) * cos(c*t),
        sin(t) * cos(c*c*t),
        sin(c*t) * cos(t)
    };

    sphere_offset *= glm::radians(half_angle);

    glm::vec3 ray_vec = -direction + sphere_offset;
    float length = std::numeric_limits<float>::infinity();
    Ray ray {shading_point, ray_vec, TRC_SHADOW_RAY};

    LightSampleData data {
        color * intensity,
        ray,
        length
    };
    return data;
}

LightSampleData SunLight::calc_ray_intersection(Ray ray) {
    glm::vec3 light {0.f};

    float solid_angle = 6.283185307f * (1.f - cos(glm::radians(half_angle)));

    if (acos(glm::dot(ray.direction, -direction)) <= glm::radians(half_angle))
        light = color * intensity / solid_angle;

    return LightSampleData {
        light,
        ray,
        std::numeric_limits<float>::infinity()
    };
}

} /* trc */
