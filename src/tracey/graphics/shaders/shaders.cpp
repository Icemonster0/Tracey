#include "shaders.hpp"

#include <optional>
#include <algorithm>

#include "../../../lib/glm.hpp"
#include "../shader_pack.hpp"
#include "../../geometry/intersection.hpp"
#include "../../engine/accelerator.hpp"
#include "../shader_data.hpp"
#include "../../util/math_util.hpp"

namespace trc {

TRC_DEFINE_SHADER(ShaderPreview) {
    float ambient = 0.2f;
    float diffuse = std::abs(glm::dot(shader_data.normal, -shader_data.ray.direction));
    float specular = diffuse*diffuse;
    float value = ambient * 0.3f + diffuse * 0.5f + specular * 0.2f;

    return glm::vec4 {glm::vec3 {value}, 1.f};
}

TRC_DEFINE_SHADER(ShaderLightDirect) {
    glm::vec3 light = shader_data.accelerator->calc_light_influence(shader_data.pos, shader_data.normal, shader_data.rng);
    return glm::vec4 {light, 1.f};
}

TRC_DEFINE_SHADER(ShaderDiffuseDirect) {
    return shader_data.shader_pack->shader_light_direct->evaluate(shader_data);
}

TRC_DEFINE_SHADER(ShaderDiffuseIndirect) {
    glm::vec3 color;

    if (shader_data.ray.index > TRC_RAY_MAX_BOUNCES) color = glm::vec4 {0.f, 0.f, 0.f, 1.f};
    else {
        Ray diffuse_ray {
            shader_data.pos,
            math::random_dir_in_hemisphere(shader_data.normal, shader_data.rng),
            TRC_DIFFUSE_RAY,
            shader_data.ray.index + 1
        };

        std::optional<Intersection> isect = shader_data.accelerator->calc_intersection(diffuse_ray);
        if (isect) {
            ShaderData diffuse_shader_data {
                isect.value().pos,
                isect.value().normal,
                isect.value().tex_coord,
                shader_data.distance + isect.value().distance,
                diffuse_ray,
                shader_data.accelerator,
                shader_data.shader_pack,
                shader_data.rng
            };
            color = isect.value().shader->evaluate(diffuse_shader_data);
        }
        else color = glm::vec4 {0.f, 0.f, 0.f, 1.f};
    }

    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderDiffuse) {
    if (glm::dot(shader_data.normal, -shader_data.ray.direction) < 0.f)
        return glm::vec4 {glm::vec3 {0.f}, 1.f};

    glm::vec3 direct = shader_data.shader_pack->shader_diffuse_direct->evaluate(shader_data).rgb();
    glm::vec3 indirect = shader_data.shader_pack->shader_diffuse_indirect->evaluate(shader_data).rgb();
    // if (shader_data.ray.type == TRC_CAMERA_RAY) direct *= 0.f;
    return glm::vec4 {direct + indirect, 1.f};
}


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

    if (shader_data.ray.index > TRC_RAY_MAX_BOUNCES || glm::dot(shader_data.normal, -shader_data.ray.direction) < 0.f)
        color = glm::vec4 {0.f, 0.f, 0.f, 1.f};
    else {
        Ray reflect_ray {
            shader_data.pos,
            glm::reflect(shader_data.ray.direction, shader_data.normal),
            TRC_SPECULAR_RAY,
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
                shader_data.shader_pack,
                shader_data.rng
            };
            color = isect.value().shader->evaluate(reflect_shader_data);
        }
        else color = glm::vec4 {0.f, 0.f, 0.f, 1.f};
    }

    color = color * 0.9f + 0.05f;
    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderShadows) {
    glm::vec3 light = shader_data.accelerator->calc_light_influence(shader_data.pos, shader_data.normal, shader_data.rng);
    return glm::vec4 {light * 0.9f + 0.1f, 1.f};
}

} /* trc */
