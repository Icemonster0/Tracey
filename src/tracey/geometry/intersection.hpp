#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "../../glm.hpp"
#include "../graphics/shader.hpp"

namespace trc {

struct Intersection {
    Intersection(glm::vec3 p_pos, glm::vec3 p_normal, glm::vec2 p_tex_coord, float p_distance, Shader *p_shader);

    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    float distance;
    Shader *shader;
};

} /* trc */

#endif /* end of include guard: INTERSECTION_HPP */
