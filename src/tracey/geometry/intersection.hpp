#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "../../lib/glm.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/material.hpp"

namespace trc {

struct Intersection {
    Intersection(glm::vec3 p_pos, glm::vec3 p_normal, glm::vec2 p_tex_coord, Material *p_material, float p_distance, Shader *p_shader);

    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    Material *material;
    float distance;
    Shader *shader;
};

} /* trc */

#endif /* end of include guard: INTERSECTION_HPP */
