#include "point_light.hpp"

#include "../../util/math_util.hpp"

namespace trc {

PointLight::PointLight(glm::vec3 p_pos, glm::vec3 p_color, float p_intensity, float p_radius)
    : Light(p_pos, p_color, p_intensity), radius(p_radius),
      random_distrib(std::uniform_real_distribution<float> {0.f, 6.283185307f}) {}

glm::vec3 PointLight::get_sample_point(RNG *rng) {
    /* surface; using gaussian distribution */
    // std::normal_distribution<float> normal_distrib;
    //
    // glm::vec3 offset {
    //      normal_distrib(*rng),
    //      normal_distrib(*rng),
    //      normal_distrib(*rng)
    // };
    //
    // offset = math::normalize(offset);
    //
    // offset *= radius;

    /* surface; using rotation */
    // std::uniform_real_distribution<float> theta_distrib {-90.f, 90.f};
    // std::uniform_real_distribution<float> phi_distrib {0.f, 360.f};
    //
    // float theta = theta_distrib(*rng);
    // float phi = phi_distrib(*rng);
    //
    // glm::vec3 offset {0.f, 0.f, -radius};
    // offset = glm::rotateX(offset, theta);
    // offset = glm::rotateY(offset, phi);

    /* volume; using rejection */
    // std::uniform_real_distribution<float> distrib {-1.f, 1.f};
    //
    // glm::vec3 offset;
    //
    // do {
    //     offset = glm::vec3 {
    //         distrib(*rng),
    //         distrib(*rng),
    //         distrib(*rng)
    //     };
    // } while (offset.x*offset.x + offset.y*offset.y + offset.z*offset.z > 1.f);
    //
    // offset *= radius;

    /* volume; using spiral */
    const int c = 10000;
    float t = random_distrib(*rng);

    glm::vec3 offset {
        sin(c*c*t) * cos(c*t),
        sin(t) * cos(c*c*t),
        sin(c*t) * cos(t)
    };

    offset *= radius;

    /* return (not part of methods above) */
    return pos + offset;
}

} /* trc */
