#ifndef SHADER_PACK_HPP
#define SHADER_PACK_HPP

#include <memory>

#include "shaders/shaders.hpp"

namespace trc {

struct ShaderPack {
    ShaderPack();

    std::unique_ptr<Shader> shader_default;
    std::unique_ptr<Shader> shader_red;
    std::unique_ptr<Shader> shader_normal;
    std::unique_ptr<Shader> shader_distance;
    std::unique_ptr<Shader> shader_tex_coord;
    std::unique_ptr<Shader> shader_basic_clay;
    std::unique_ptr<Shader> shader_checker;
    std::unique_ptr<Shader> shader_reflect;
};

} /* trc */

#endif /* end of include guard: SHADER_PACK_HPP */
