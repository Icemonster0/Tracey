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

glm::vec3 ggx_inverse(glm::vec3 out, glm::vec3 g_normal, float rough, RNG *rng) {
    std::uniform_real_distribution<float> distrib {0.f, 1.f};
    float epsilon = distrib(*rng);

    float theta = std::atan(rough * std::sqrt(epsilon / (1.f - epsilon)));
    float phi = distrib(*rng) * 2*PI;

    glm::vec3 tangent = glm::cross(g_normal == glm::vec3 {0.f, 1.f, 0.f} ? glm::vec3 {0.f, 0.f, 1.f} : glm::vec3 {0.f, 1.f, 0.f}, g_normal);
    glm::vec3 m_normal = glm::rotate(g_normal, theta, tangent);
    m_normal = glm::rotate(m_normal, phi, g_normal);

    return glm::reflect(-out, m_normal);
}

} /* trc::brdf */
