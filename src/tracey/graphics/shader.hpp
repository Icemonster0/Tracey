#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../glm.hpp"

namespace trc {

struct Shader {
    virtual glm::vec4 evaluate(
        glm::vec3 pos,
        glm::vec3 normal,
        glm::vec2 tex_coord,
        float distance
    );
};

} /* trc */

#endif /* end of include guard: SHADER_HPP */
