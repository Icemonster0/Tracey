#include "shader_pack.hpp"

namespace trc {

ShaderPack::ShaderPack()
    : shader_default(std::unique_ptr<Shader>(new Shader())),
      shader_preview(std::unique_ptr<Shader>(new ShaderPreview())),
      shader_diffuse_direct(std::unique_ptr<Shader>(new ShaderDiffuseDirect())),
      shader_diffuse_indirect(std::unique_ptr<Shader>(new ShaderDiffuseIndirect())),
      shader_specular_direct(std::unique_ptr<Shader>(new ShaderSpecularDirect())),
      shader_specular_indirect(std::unique_ptr<Shader>(new ShaderSpecularIndirect())),
      shader_transmission(std::unique_ptr<Shader>(new ShaderTransmission())),
      shader_combined(std::unique_ptr<Shader>(new ShaderCombined())),

      shader_red(std::unique_ptr<Shader>(new ShaderRed())),
      shader_normal(std::unique_ptr<Shader>(new ShaderNormal())),
      shader_distance(std::unique_ptr<Shader>(new ShaderDistance())),
      shader_tex_coord(std::unique_ptr<Shader>(new ShaderTexCoord())),
      shader_basic_clay(std::unique_ptr<Shader>(new ShaderBasicClay())),
      shader_checker(std::unique_ptr<Shader>(new ShaderChecker())),
      shader_reflect(std::unique_ptr<Shader>(new ShaderReflect())),
      shader_shadows(std::unique_ptr<Shader>(new ShaderShadows())) {}

} /* trc */
