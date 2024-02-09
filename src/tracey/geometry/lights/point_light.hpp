#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "../light.hpp"

namespace trc {

class PointLight : public Light {
public:
    PointLight(glm::vec3 p_pos, glm::vec3 p_color = glm::vec3 {1.f}, float p_intensity = 1.f, float p_radius = 0.1f);

protected:
    glm::vec3 get_sample_point();

private:
    flaot radius;
};

} /* trc */

#endif /* end of include guard: POINT_LIGHT_HPP */
