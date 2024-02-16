#include "brdfs.hpp"

#include "../util/math_util.hpp"

namespace trc::brdf {

const float PI = 3.141592654f;
const float INV_PI = 0.318309886f;

float flat(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough) {
    return INV_PI;
}

float ggx(glm::vec3 in, glm::vec3 out, glm::vec3 normal, float rough) {
    glm::vec3 half = math::normalize(in + out);
    float cos_theta_m = glm::dot(normal, half);
    float cos_squared = cos_theta_m*cos_theta_m;
    float tan_squared = (1.f - cos_squared) / cos_squared;
    float value = rough / (cos_squared * (rough*rough + tan_squared));
    return INV_PI * value*value;
}

glm::vec3 ggx_normal(glm::vec3 g_normal, float rough, RNG *rng) {
    std::uniform_real_distribution<float> distrib {0.f, 1.f};
    float epsilon = distrib(*rng);
    float theta = std::atan(rough * std::sqrt(epsilon / (1.f - epsilon)));

    return randomize_ray(g_normal, theta, rng);
}

glm::vec3 randomize_ray(glm::vec3 ray, float theta, RNG *rng) {
    /* rotates ray on a random tangent by theta radians */

    std::uniform_real_distribution<float> distrib {0.f, 2*PI};
    float phi = distrib(*rng);

    glm::vec3 tangent = glm::cross(ray == glm::vec3 {0.f, 1.f, 0.f} ? glm::vec3 {0.f, 0.f, 1.f} : glm::vec3 {0.f, 1.f, 0.f}, ray);
    glm::vec3 new_ray = glm::rotate(ray, theta, tangent);
    return glm::rotate(new_ray, phi, ray);
}

} /* trc::brdf */
