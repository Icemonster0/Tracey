#ifndef SHADER_HPP
#define SHADER_HPP

#include "../../glm.hpp"
#include "shader_data.hpp"

namespace trc {

struct Shader {
    virtual glm::vec4 evaluate(ShaderData shader_data) const;
};

} /* trc */

#endif /* end of include guard: SHADER_HPP */
