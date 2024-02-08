#include "shaders.hpp"

#include <optional>

#include "../../../glm.hpp"
#include "../shader_pack.hpp"
#include "../../geometry/intersection.hpp"
#include "../../engine/accelerator.hpp"
#include "../shader_data.hpp"

namespace trc {

TRC_DEFINE_SHADER(ShaderRed) {
    return glm::vec4 {1.f, 0.f, 0.f, 1.f};
}

TRC_DEFINE_SHADER(ShaderNormal) {
    return glm::vec4 {shader_data.normal, 1.f};
}

TRC_DEFINE_SHADER(ShaderDistance) {
    return glm::vec4 {glm::vec3 {1 - (shader_data.distance / 10.f)}, 1.f};
}

TRC_DEFINE_SHADER(ShaderTexCoord) {
    return glm::vec4 {shader_data.tex_coord, 0.f, 1.f};
}

TRC_DEFINE_SHADER(ShaderBasicClay) {
    return glm::vec4 {glm::vec3 {
        glm::clamp(-glm::dot(shader_data.ray.direction, shader_data.normal), 0.f, 1.f) * 0.8f + 0.2f
    }, 1.f};
}

TRC_DEFINE_SHADER(ShaderChecker) {
    float fac = shader_data.shader_pack->shader_basic_clay->evaluate(shader_data).r;
    shader_data.tex_coord *= 1.f;
    bool is_white = (glm::fract(shader_data.tex_coord.s) < 0.5f) != (glm::fract(shader_data.tex_coord.t) < 0.5f);
    return glm::vec4 {glm::vec3 {(is_white ? 1.f : 0.f) * fac}, 1.f};
}

TRC_DEFINE_SHADER(ShaderReflect) {
    glm::vec3 color;

    if (shader_data.ray.index > TRC_RAY_MAX_BOUNCES) color = glm::vec4 {0.f, 0.f, 0.f, 1.f};
    else {
        Ray reflect_ray {
            shader_data.pos,
            glm::reflect(shader_data.ray.direction, shader_data.normal),
            TRC_COMMON_RAY,
            shader_data.ray.index + 1
        };

        std::optional<Intersection> isect = shader_data.accelerator->calc_intersection(reflect_ray);
        if (isect) {
            ShaderData reflect_shader_data {
                isect.value().pos,
                isect.value().normal,
                isect.value().tex_coord,
                shader_data.distance + isect.value().distance,
                reflect_ray,
                shader_data.accelerator,
                shader_data.shader_pack
            };
            color = isect.value().shader->evaluate(reflect_shader_data);
        }
        else color = glm::vec4 {0.f, 0.f, 0.f, 1.f};
    }

    color = color * 0.8f + 0.1f;
    return glm::vec4 {color, 1.f};
}

} /* trc */
