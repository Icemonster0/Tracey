#ifndef SHADER_DATA_HPP
#define SHADER_DATA_HPP

#include "../../glm.hpp"
#include "../geometry/ray.hpp"
// #include "../engine/accelerator.hpp"
// #include "shader_pack.hpp"

namespace trc {

class Accelerator;
struct ShaderPack;

struct ShaderData {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    float distance;
    Ray ray;
    Accelerator *accelerator;
    ShaderPack *shader_pack;
};

} /* trc */

#endif /* end of include guard: SHADER_DATA_HPP */
