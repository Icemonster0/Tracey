#include "light.hpp"

namespace trc {

Light::Light(glm::vec3 p_pos, glm::vec3 p_color, float p_intensity)
    : pos(p_pos), color(p_color), intensity(p_intensity) {}

LightSampleData Light::sample(glm::vec3 shading_point, RNG *rng) {
    glm::vec3 ray_vec = get_sample_point(rng) - shading_point;
    float length = glm::length(ray_vec);
    float inv_length = 1.f / length;
    Ray ray {shading_point, ray_vec * inv_length, TRC_SHADOW_RAY};
    float falloff = inv_length*inv_length;

    LightSampleData data {
        color * intensity * falloff,
        ray,
        length
    };
    return data;
}

LightSampleData Light::calc_ray_intersection(Ray ray) {
    return LightSampleData {
        glm::vec3 {0.f},
        ray,
        std::numeric_limits<float>::infinity()
    };
}

glm::vec3 Light::get_sample_point(RNG *rng) {
    return pos;
}

} /* trc */
