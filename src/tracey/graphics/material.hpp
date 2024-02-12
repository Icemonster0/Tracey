#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>

#include "../../lib/glm.hpp"
#include "attribute.hpp"

namespace trc {

struct Material {
    Material(glm::vec3 a, float r = 0.5f, float m = 0.f, glm::vec3 e = glm::vec3 {0.f}, glm::vec3 n = glm::vec3(0.5f, 0.5f, 1.f));
    Material();

    std::shared_ptr<Attrib<glm::vec3>> albedo;
    std::shared_ptr<Attrib<float>> roughness;
    std::shared_ptr<Attrib<float>> metallic;
    std::shared_ptr<Attrib<glm::vec3>> emission;
    std::shared_ptr<Attrib<glm::vec3>> normal;
};

} /* trc */

#endif /* end of include guard: MATERIAL_HPP */
