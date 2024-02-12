#include "shaders.hpp"

#include <optional>
#include <algorithm>

#include "../../../lib/glm.hpp"
#include "../shader_pack.hpp"
#include "../../geometry/intersection.hpp"
#include "../../engine/accelerator.hpp"
#include "../shader_data.hpp"
#include "../../util/math_util.hpp"
#include "../../graphics/brdfs.hpp"

namespace trc {

#define SAMPLE_ATTRIB(attrib) shader_data.material->attrib->sample(shader_data.tex_coord)
#define EVALUATE_SHADER(shader_name, shader_data) shader_data.shader_pack->shader_name->evaluate(shader_data)

TRC_DEFINE_SHADER(ShaderPreview) {
    float ambient = 0.2f;
    float diffuse = std::abs(glm::dot(shader_data.normal, -shader_data.ray.direction));
    float specular = diffuse*diffuse;
    float value = ambient * 0.3f + diffuse * 0.5f + specular * 0.2f;

    return glm::vec4 {glm::vec3 {value}, 1.f};
}

TRC_DEFINE_SHADER(ShaderDiffuseDirect) {
    glm::vec3 light = shader_data.accelerator->calc_light_influence(
        shader_data.pos, // shading point
        shader_data.normal, // normal
        -shader_data.ray.direction, // outgoing direction
        SAMPLE_ATTRIB(roughness), // roughness
        shader_data.rng, // rng
        brdf::flat // brdf
    );
    return glm::vec4 {light, 1.f};
}

TRC_DEFINE_SHADER(ShaderDiffuseIndirect) {
    glm::vec3 color;

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
            isect.value().material,
            shader_data.distance + isect.value().distance,
            diffuse_ray,
            shader_data.accelerator,
            shader_data.shader_pack,
            shader_data.rng
        };
        color = isect.value().shader->evaluate(diffuse_shader_data);
    }
    else color = glm::vec3 {0.f, 0.f, 0.f};

    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderSpecularDirect) {
    glm::vec3 light = shader_data.accelerator->calc_light_influence(
        shader_data.pos, // shading point
        shader_data.normal, // normal
        -shader_data.ray.direction, // outgoing direction
        SAMPLE_ATTRIB(roughness), // roughness
        shader_data.rng, // rng
        brdf::ggx // brdf
    );
    return glm::vec4 {light, 1.f};
}

TRC_DEFINE_SHADER(ShaderSpecularIndirect) {
    glm::vec3 color;

    Ray specular_ray {
        shader_data.pos,
        brdf::ggx_inverse(
            -shader_data.ray.direction, // outgoing direction
            shader_data.normal, // normal
            SAMPLE_ATTRIB(roughness), // roughness
            shader_data.rng // rng
        ),
        TRC_SPECULAR_RAY,
        shader_data.ray.index + 1
    };

    std::optional<Intersection> isect = shader_data.accelerator->calc_intersection(specular_ray);
    if (isect) {
        ShaderData specular_shader_data {
            isect.value().pos,
            isect.value().normal,
            isect.value().tex_coord,
            isect.value().material,
            shader_data.distance + isect.value().distance,
            specular_ray,
            shader_data.accelerator,
            shader_data.shader_pack,
            shader_data.rng
        };
        color = isect.value().shader->evaluate(specular_shader_data);
    }
    else color = glm::vec3 {0.f, 0.f, 0.f};

    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderCombined) {
    if (glm::dot(shader_data.normal, -shader_data.ray.direction) < 0.f)
        return glm::vec4 {glm::vec3 {0.f}, 1.f};

    glm::vec3 diffuse_direct = EVALUATE_SHADER(shader_diffuse_direct, shader_data).rgb();
    glm::vec3 specular_direct = EVALUATE_SHADER(shader_specular_direct, shader_data).rgb();

    glm::vec3 diffuse_indirect {0.f};
    glm::vec3 specular_indirect {0.f};
    if (shader_data.ray.index <= TRC_RAY_MAX_BOUNCES) {
        diffuse_indirect = EVALUATE_SHADER(shader_diffuse_indirect, shader_data).rgb();
        specular_indirect = EVALUATE_SHADER(shader_specular_indirect, shader_data).rgb();
    }

    glm::vec3 albedo = SAMPLE_ATTRIB(albedo);
    float metallic = SAMPLE_ATTRIB(metallic);
    glm::vec3 diffuse_color {albedo * (1.f - metallic)};
    glm::vec3 specular_color {math::fresnel(1.f, 2.0f, -shader_data.ray.direction, shader_data.normal) + albedo * metallic};

    glm::vec3 diffuse = (diffuse_direct + diffuse_indirect) * diffuse_color;
    glm::vec3 specular = (specular_direct + specular_indirect) * specular_color;

    glm::vec3 emission {SAMPLE_ATTRIB(emission)};

    glm::vec3 combined = diffuse + specular + emission;
    return glm::vec4 {combined, 1.f};
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
                isect.value().material,
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
    glm::vec3 light = shader_data.accelerator->calc_light_influence(
        shader_data.pos,
        shader_data.normal,
        -shader_data.ray.direction,
        shader_data.material->roughness->sample(shader_data.tex_coord),
        shader_data.rng,
        brdf::flat
    );
    return glm::vec4 {light * 0.9f + 0.1f, 1.f};
}

#undef SAMPLE_ATTRIB
#undef EVALUATE_SHADER

} /* trc */
