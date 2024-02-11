#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../../lib/glm.hpp"
#include "ray.hpp"
#include "../util/random.hpp"

namespace trc {

struct LightSampleData {
    glm::vec3 light;
    Ray shadow_ray;
    float distance;
};

class Light {
public:
    Light(glm::vec3 p_pos, glm::vec3 p_color = glm::vec3 {1.f}, float p_intensity = 1.f);

    virtual LightSampleData sample(glm::vec3 shading_point, RNG *rng);

protected:
    virtual glm::vec3 get_sample_point(RNG *rng);

    glm::vec3 pos;
    glm::vec3 color;
    float intensity;
};

} /* trc */

#endif /* end of include guard: LIGHT_HPP */
