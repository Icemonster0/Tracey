#ifndef SHADER_DATA_HPP
#define SHADER_DATA_HPP

#include <memory>

#include "../../lib/glm.hpp"
#include "../geometry/ray.hpp"
#include "../util/random.hpp"
#include "material.hpp"

namespace trc {

class Accelerator;
struct ShaderPack;

struct ShaderData {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec2 tex_coord;
    Material *material;
    float distance;
    Ray ray;
    int max_bounces;
    Accelerator *accelerator;
    ShaderPack *shader_pack;
    RNG *rng;
};

} /* trc */

#endif /* end of include guard: SHADER_DATA_HPP */
