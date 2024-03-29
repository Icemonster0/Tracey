#ifndef SUN_LIGHT_HPP
#define SUN_LIGHT_HPP

#include "../light.hpp"

namespace trc {

class SunLight : public Light {
public:
    SunLight(glm::vec3 p_direction, glm::vec3 p_color = glm::vec3 {1.f}, float p_intensity = 3.f, float p_angle = 0.526f);

    LightSampleData sample(glm::vec3 shading_point, RNG *rng);
    LightSampleData calc_ray_intersection(Ray ray);

private:
    float half_angle;
    glm::vec3 direction;
    std::uniform_real_distribution<float> random_distrib;
};

} /* trc */

#endif /* end of include guard: SUN_LIGHT_HPP */
