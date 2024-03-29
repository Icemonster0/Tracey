#ifndef SHADERS_HPP
#define SHADERS_HPP

#include "../shader.hpp"
#include "shader_syntax.hpp"

namespace trc {

TRC_DECLARE_SHADER(ShaderPreview)
TRC_DECLARE_SHADER(ShaderDiffuseDirect)
TRC_DECLARE_SHADER(ShaderDiffuseIndirect)
TRC_DECLARE_SHADER(ShaderSpecularDirect)
TRC_DECLARE_SHADER(ShaderSpecularIndirect)
TRC_DECLARE_SHADER(ShaderTransmission)
TRC_DECLARE_SHADER(ShaderTransparent)
TRC_DECLARE_SHADER(ShaderCombined)

TRC_DECLARE_SHADER(ShaderRed)
TRC_DECLARE_SHADER(ShaderNormal)
TRC_DECLARE_SHADER(ShaderDistance)
TRC_DECLARE_SHADER(ShaderTexCoord)
TRC_DECLARE_SHADER(ShaderBasicClay)
TRC_DECLARE_SHADER(ShaderChecker)
TRC_DECLARE_SHADER(ShaderReflect)
TRC_DECLARE_SHADER(ShaderShadows)

} /* trc */

#endif /* end of include guard: SHADERS_HPP */
