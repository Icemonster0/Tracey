#include "light.hpp"

namespace trc {

Light::Light(glm::vec3 p_pos, glm::vec3 p_color, float p_intensity)
    : pos(p_pos), color(p_color), intensity(p_intensity) {}

LightSampleData Light::sample(glm::vec3 shading_point) {
    glm::vec3 ray_vec = pos - shading_point;
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

glm::vec3 Light::get_sample_point() {
    return pos;
}

} /* trc */
