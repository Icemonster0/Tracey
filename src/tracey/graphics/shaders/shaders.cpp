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
#define EVALUATE_SHADER(shader_name, new_shader_data) shader_data.shader_pack->shader_name->evaluate(new_shader_data)

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
        math::random_dir_in_hemisphere(shader_data.normal, shader_data.rng), // brdf normal reflection direction (for flat distribution)
        TRC_DIFFUSE_RAY,
        shader_data.ray.index + 1
    };

    std::optional<Intersection> isect = shader_data.accelerator->calc_intersection(diffuse_ray);
    if (isect) {
        ShaderData diffuse_shader_data {
            isect.value().pos,
            isect.value().normal,
            isect.value().tan,
            isect.value().bitan,
            isect.value().tex_coord,
            isect.value().material,
            shader_data.distance + isect.value().distance,
            diffuse_ray,
            shader_data.max_bounces,
            shader_data.accelerator,
            shader_data.shader_pack,
            shader_data.rng
        };
        color = isect.value().shader->evaluate(diffuse_shader_data);
        color = glm::min(color, {TRC_INDIRECT_LIGHT_CLAMP});
    }
    else color = shader_data.accelerator->get_environment_light(diffuse_ray);

    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderSpecularDirect) {
    float roughness = SAMPLE_ATTRIB(roughness);

    if (roughness < TRC_SPECULAR_OPTIMIZE_TRESHOLD)
        return glm::vec4 {glm::vec3 {0.f}, 1.f};

    glm::vec3 light = shader_data.accelerator->calc_light_influence(
        shader_data.pos, // shading point
        shader_data.normal, // normal
        -shader_data.ray.direction, // outgoing direction
        roughness, // roughness
        shader_data.rng, // rng
        brdf::ggx // brdf
    );
    return glm::vec4 {light, 1.f};
}

TRC_DEFINE_SHADER(ShaderSpecularIndirect) {
    glm::vec3 color;

    float roughness = SAMPLE_ATTRIB(roughness);
    bool include_lights = (roughness < TRC_SPECULAR_OPTIMIZE_TRESHOLD);
    Ray specular_ray {glm::vec3 {0}, glm::vec3 {0}};

    specular_ray = Ray {
        shader_data.pos,
        glm::reflect(shader_data.ray.direction, brdf::ggx_normal(
            shader_data.normal,
            shader_data.tangent,
            roughness,
            shader_data.rng
        )),
        TRC_SPECULAR_RAY,
        shader_data.ray.index + 1
    };

    if (glm::dot(specular_ray.direction, shader_data.normal) < 0.f) {
        return glm::vec4 {0.f};
    }

    float obj_isect_distance = std::numeric_limits<float>::infinity();
    std::optional<Intersection> obj_isect = shader_data.accelerator->calc_intersection(specular_ray);
    if (obj_isect) {
        obj_isect_distance = obj_isect.value().distance;
        ShaderData specular_shader_data {
            obj_isect.value().pos,
            obj_isect.value().normal,
            obj_isect.value().tan,
            obj_isect.value().bitan,
            obj_isect.value().tex_coord,
            obj_isect.value().material,
            shader_data.distance + obj_isect_distance,
            specular_ray,
            shader_data.max_bounces,
            shader_data.accelerator,
            shader_data.shader_pack,
            shader_data.rng
        };
        color = obj_isect.value().shader->evaluate(specular_shader_data);
        color = glm::min(color, {TRC_INDIRECT_LIGHT_CLAMP});
    }
    else color = shader_data.accelerator->get_environment_light(specular_ray);

    if (include_lights) {
        LightSampleData light_isect = shader_data.accelerator->calc_light_intersection(specular_ray);
        if (light_isect.distance <= obj_isect_distance) {
            color += light_isect.light;
        }
    }

    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderTransmission) {
    glm::vec3 indirect {0.f};
    glm::vec3 direct {0.f};

    float roughness = SAMPLE_ATTRIB(roughness);
    float ior = SAMPLE_ATTRIB(ior);
    float ior_ratio;
    float normal_flipper;

    if (glm::dot(shader_data.normal, -shader_data.ray.direction) < 0.f) { // if backfacing
        ior_ratio = ior;
        normal_flipper = -1.f;

        direct = shader_data.accelerator->calc_light_influence(
            shader_data.pos, // shading point
            shader_data.normal, // normal
            -shader_data.ray.direction, // outgoing direction
            roughness, // roughness
            shader_data.rng, // rng
            brdf::ggx // brdf
        );
    } else {
        ior_ratio = 1.f / ior;
        normal_flipper = 1.f;
    }

    glm::vec3 refract_dir = glm::refract(
        shader_data.ray.direction,
        brdf::ggx_normal(
            shader_data.normal,
            shader_data.tangent,
            roughness,
            shader_data.rng
        ) * normal_flipper,
        ior_ratio
    );

    Ray transmit_ray {
        shader_data.pos,
        refract_dir,
        TRC_TRANSMISSION_RAY,
        shader_data.ray.index + 1
    };

    float obj_isect_distance = std::numeric_limits<float>::infinity();
    std::optional<Intersection> obj_isect = shader_data.accelerator->calc_intersection(transmit_ray);
    if (obj_isect) {
        obj_isect_distance = obj_isect.value().distance;
        ShaderData transmit_shader_data {
            obj_isect.value().pos,
            obj_isect.value().normal,
            obj_isect.value().tan,
            obj_isect.value().bitan,
            obj_isect.value().tex_coord,
            obj_isect.value().material,
            shader_data.distance + obj_isect_distance,
            transmit_ray,
            shader_data.max_bounces,
            shader_data.accelerator,
            shader_data.shader_pack,
            shader_data.rng
        };
        indirect = obj_isect.value().shader->evaluate(transmit_shader_data).rgb();
    }
    else indirect = shader_data.accelerator->get_environment_light(transmit_ray);

    LightSampleData light_isect = shader_data.accelerator->calc_light_intersection(transmit_ray);
    if (light_isect.distance <= obj_isect_distance) {
        indirect += light_isect.light;
    }

    return glm::vec4 {indirect + direct, 1.f};
}

TRC_DEFINE_SHADER(ShaderTransparent) {
    glm::vec3 color;

    Ray through_ray {
        shader_data.pos,
        shader_data.ray.direction,
        TRC_TRANSPARENT_RAY,
        shader_data.ray.index // not incrementing to allow infinite transparent bounces
    };

    std::optional<Intersection> isect = shader_data.accelerator->calc_intersection(through_ray);
    if (isect) {
        ShaderData isect_shader_data {
            isect.value().pos,
            isect.value().normal,
            isect.value().tan,
            isect.value().bitan,
            isect.value().tex_coord,
            isect.value().material,
            shader_data.distance + isect.value().distance,
            through_ray,
            shader_data.max_bounces,
            shader_data.accelerator,
            shader_data.shader_pack,
            shader_data.rng
        };
        color = isect.value().shader->evaluate(isect_shader_data);
    }
    else color = shader_data.accelerator->get_environment_light(through_ray);

    return glm::vec4 {color, 1.f};
}

TRC_DEFINE_SHADER(ShaderCombined) {
    glm::vec3 diffuse {0.f};
    glm::vec3 specular {0.f};
    glm::vec3 transmission {0.f};
    glm::vec3 transparent {0.f};

    glm::vec3 tan_space_normal = SAMPLE_ATTRIB(normal)*2.f-1.f;
    shader_data.normal = glm::vec3 {
        shader_data.tangent * tan_space_normal.x +
        shader_data.bitangent * tan_space_normal.y +
        shader_data.normal * tan_space_normal.z
    };
    glm::vec3 albedo = SAMPLE_ATTRIB(albedo);
    float metallic = SAMPLE_ATTRIB(metallic);
    float alpha = SAMPLE_ATTRIB(alpha);

    if (SAMPLE_ATTRIB(transmissive) > 0.f) {
        // transmission
        glm::vec3 transmission_indirect {0.f};
        if (shader_data.ray.index < shader_data.max_bounces) {
            transmission_indirect = EVALUATE_SHADER(shader_transmission, shader_data).rgb();
        }
        glm::vec3 transmission_color {glm::saturate(albedo)}; // saturate = clamp between 0 and 1
        transmission = transmission_indirect * transmission_color;
    }
    else {
        // invert backfacing normals
        if (glm::dot(shader_data.normal, -shader_data.ray.direction) < 0.f) {
            shader_data.normal *= -1.f;
        }

        // diffuse
        glm::vec3 diffuse_direct = EVALUATE_SHADER(shader_diffuse_direct, shader_data).rgb();
        glm::vec3 diffuse_indirect {0.f};
        if (shader_data.ray.index < shader_data.max_bounces) {
            diffuse_indirect = EVALUATE_SHADER(shader_diffuse_indirect, shader_data).rgb();
        }
        glm::vec3 diffuse_color {glm::saturate(albedo * (1.f - metallic))};
        diffuse = (diffuse_direct + diffuse_indirect) * diffuse_color;
    }

    // specular
    float fresnel = math::fresnel(1.f, SAMPLE_ATTRIB(ior), -shader_data.ray.direction, shader_data.normal);
    glm::vec3 specular_direct = EVALUATE_SHADER(shader_specular_direct, shader_data).rgb();
    glm::vec3 specular_indirect {0.f};
    if (shader_data.ray.index < shader_data.max_bounces) {
        specular_indirect = EVALUATE_SHADER(shader_specular_indirect, shader_data).rgb();
    }
    glm::vec3 specular_color {glm::saturate(fresnel + (albedo * metallic))};
    specular = (specular_direct + specular_indirect) * specular_color;

    // emission
    glm::vec3 emission {SAMPLE_ATTRIB(emission)};

    // transparency
    if (alpha < 1.f) {
        transparent = EVALUATE_SHADER(shader_transparent, shader_data).rgb();
    }

    // combine
    glm::vec3 combined = diffuse + specular + transmission + emission;
    combined = glm::mix(transparent, combined, alpha);

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

    if (shader_data.ray.index >= shader_data.max_bounces || glm::dot(shader_data.normal, -shader_data.ray.direction) < 0.f)
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
                isect.value().tan,
                isect.value().bitan,
                isect.value().tex_coord,
                isect.value().material,
                shader_data.distance + isect.value().distance,
                reflect_ray,
                shader_data.max_bounces,
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
