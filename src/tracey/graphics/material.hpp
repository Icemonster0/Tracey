#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>

#include "../../lib/glm.hpp"
#include "attribute.hpp"

namespace trc {

struct Material {
    Material(glm::vec3 a, float r = 0.5f, float m = 0.f, glm::vec3 e = glm::vec3 {0.f}, glm::vec3 n = glm::vec3 {0.5f, 0.5f, 1.f}, float t = 0.f, float i = 1.6f, float o = 1.f);
    Material(std::shared_ptr<Attrib<glm::vec3>> a,
             std::shared_ptr<Attrib<float>> r,
             std::shared_ptr<Attrib<float>> m,
             std::shared_ptr<Attrib<glm::vec3>> e,
             std::shared_ptr<Attrib<glm::vec3>> n,
             std::shared_ptr<Attrib<float>> t,
             std::shared_ptr<Attrib<float>> i,
             std::shared_ptr<Attrib<float>> o);
    Material();

    std::shared_ptr<Attrib<glm::vec3>> albedo;
    std::shared_ptr<Attrib<float>> roughness;
    std::shared_ptr<Attrib<float>> metallic;
    std::shared_ptr<Attrib<glm::vec3>> emission;
    std::shared_ptr<Attrib<glm::vec3>> normal;
    std::shared_ptr<Attrib<float>> transmissive;
    std::shared_ptr<Attrib<float>> ior;
    std::shared_ptr<Attrib<float>> alpha;

    void clamp_attribs();
};

} /* trc */

#endif /* end of include guard: MATERIAL_HPP */
