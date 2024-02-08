#include "shader_pack.hpp"

namespace trc {

ShaderPack::ShaderPack()
    : shader_default(std::unique_ptr<Shader>(new Shader())),
      shader_red(std::unique_ptr<Shader>(new ShaderRed())),
      shader_normal(std::unique_ptr<Shader>(new ShaderNormal())),
      shader_distance(std::unique_ptr<Shader>(new ShaderDistance())),
      shader_tex_coord(std::unique_ptr<Shader>(new ShaderTexCoord())),
      shader_basic_clay(std::unique_ptr<Shader>(new ShaderBasicClay())),
      shader_checker(std::unique_ptr<Shader>(new ShaderChecker())),
      shader_reflect(std::unique_ptr<Shader>(new ShaderReflect())) {}

} /* trc */
