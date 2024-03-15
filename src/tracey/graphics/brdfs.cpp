#include "brdfs.hpp"

#include "../util/math_util.hpp"

namespace trc::brdf {

const float PI = 3.141592654f;
const float INV_PI = 0.318309886f;

float flat(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough) {
    return INV_PI;
}

float ggx(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough) {
    float a = rough*rough;
    glm::vec3 half = math::normalize(in + out);
    float cos_theta = glm::dot(normal, half);
    float value = a / (cos_theta*cos_theta * (a*a - 1.f) + 1.f);
    return INV_PI * value*value * a;
}

glm::vec3 ggx_normal(glm::vec3 g_normal, glm::vec3 g_tangent, float rough, RNG *rng) {
    float a = rough*rough;
    std::uniform_real_distribution<float> distrib {0.000001f, 0.999999f};
    float epsilon = distrib(*rng);
    float theta = std::atan(a * std::sqrt(epsilon / (1.f - epsilon)));
    // float theta = std::atan(rough * std::sqrt(epsilon / (1.f - epsilon))) / 2.f;

    return randomize_ray(g_normal, g_tangent, theta, rng);
}

glm::vec3 randomize_ray(glm::vec3 ray, glm::vec3 tangent_ray, float theta, RNG *rng) {
    /* rotates ray on a random tangent by theta radians */

    std::uniform_real_distribution<float> distrib {0.f, 2*PI};
    float phi = distrib(*rng);

    glm::vec3 new_ray = glm::rotate(ray, theta, tangent_ray);

    return glm::rotate(new_ray, phi, ray);
}

} /* trc::brdf */
