#include "math_util.hpp"

namespace trc::math {

float fast_inv_sqrt(float number) {
    union {
        float f;
        uint32_t i;
    } conv;

    float x2;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    conv.f = number;
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
    return conv.f;
}

glm::vec3 random_dir_in_hemisphere(glm::vec3 normal, RNG *rng) {
    std::normal_distribution distrib;

    glm::vec3 dir {
        distrib(*rng),
        distrib(*rng),
        distrib(*rng)
    };

    dir *= glm::sign(glm::dot(dir, normal));

    return dir * fast_inv_sqrt(dir.x*dir.x + dir.y*dir.y * dir.z*dir.z);
}

} /* trc::math */
