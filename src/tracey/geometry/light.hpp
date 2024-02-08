#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../../glm.hpp"
#include "ray.hpp"

namespace trc {

struct LightSampleData {
    glm::vec3 light;
    Ray shadow_ray;
    float distance;
};

class Light {
public:
    Light(glm::vec3 p_pos, glm::vec3 p_color, float p_intensity);

    virtual LightSampleData sample(glm::vec3 shading_point);

protected:
    virtual glm::vec3 get_sample_point();

private:
    glm::vec3 pos;
    glm::vec3 color;
    float intensity;
};

} /* trc */

#endif /* end of include guard: LIGHT_HPP */
