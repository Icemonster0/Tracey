#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../../glm.hpp"
#include "ray.hpp"

namespace trc {

struct LightSampleData {
    glm::vec3 pos;
    glm::vec3 light;
    Ray shadow_ray;
};

class Light {
public:
    Light(glm::vec3 p_pos);

    virtual LightSampleData sample_light(glm::vec3 shading_point);

private:
    glm::vec3 pos;
};

} /* trc */

#endif /* end of include guard: LIGHT_HPP */
