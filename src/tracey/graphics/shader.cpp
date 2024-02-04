#include "shader.hpp"

namespace trc {

glm::vec4 Shader::evaluate(
    glm::vec3 pos,
    glm::vec3 normal,
    glm::vec2 tex_coord,
    float distance
) {
    // return glm::vec4 {1.f};
    // return glm::vec4 {pos, 1.f};
    return glm::vec4 {normal*0.5f + 0.5f, 1.f};
    // return glm::vec4 {tex_coord.rg(), 0.f, 1.f};
    // return glm::vec4 {glm::vec3 {1 - (distance / 10.f)}, 1.f};
}

} /* trc */
