#ifndef SHADER_DATA_HPP
#define SHADER_DATA_HPP

#include "../../lib/glm.hpp"
#include "../geometry/ray.hpp"
#include "../util/random.hpp"

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
    RNG *rng;
};

} /* trc */

#endif /* end of include guard: SHADER_DATA_HPP */
